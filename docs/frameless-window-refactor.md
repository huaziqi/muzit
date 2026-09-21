# 无边框窗口移动与缩放改造说明

## 改造目标

这次改造不增加 Muzit 的业务功能，只处理无边框窗口的交互和窗口缩放时的卡顿问题。

原实现由应用自己根据鼠标坐标调用 `move()`、`resize()` 和 `setGeometry()`。这种做法绕过了操作系统的窗口移动/缩放循环，容易在快速拖动、多显示器、不同 DPI 和最小尺寸边界处发生跳动。

改造后的原则是：

- 应用只判断用户正在拖动标题栏还是哪一条窗口边缘。
- 实际移动与缩放交给 Qt 的系统窗口接口。
- 最大化、还原和最小化使用标准窗口状态。
- 窗口连续缩放期间，不重复创建歌曲卡片，不在每个像素变化时缩放所有封面。

## 一、窗口移动

修改文件：`src/ui/window/titlebar.cpp`

标题栏原来保存鼠标按下位置和窗口起始位置，然后在每次 `mouseMoveEvent()` 中调用 `window()->move()`。

现在标题栏按下时调用：

```cpp
QWindow *handle = window()->windowHandle();
if (handle && handle->startSystemMove()) {
    event->accept();
    return;
}
```

`startSystemMove()` 会让 Windows/Qt 接管后续移动过程，因此不再需要保存 `mouseStartPoint`、`windowStartPoint` 和 `isMousePressed`，也不再需要标题栏自己的 `mouseMoveEvent()`、`mouseReleaseEvent()`。

收益：

- 移动由系统合成和调度，快速拖动更加稳定。
- 支持系统贴边吸附。
- 跨显示器移动和不同缩放比例由系统处理。

标题栏还增加了双击最大化/还原，与常规桌面窗口行为一致。

## 二、窗口边缘缩放

修改文件：

- `src/ui/window/framelesswidget.h`
- `src/ui/window/framelesswidget.cpp`

### 边缘判断

`resizeEdgesAt()` 根据鼠标在窗口内的局部坐标生成 `Qt::Edges`：

- 左右边缘分别对应 `Qt::LeftEdge` 和 `Qt::RightEdge`。
- 上下边缘分别对应 `Qt::TopEdge` 和 `Qt::BottomEdge`。
- 四个角由两个方向组合，例如左上角是 `Qt::LeftEdge | Qt::TopEdge`。

当前可拖动边框宽度由文件顶部的常量控制：

```cpp
constexpr int kResizeBorder = 6;
```

如果以后需要扩大或缩小边缘热区，只修改这个常量即可。它同时用于内容外边距，确保边缘区域不会被子控件覆盖。

### 启动系统缩放

鼠标左键按下边缘后执行：

```cpp
windowHandle()->startSystemResize(edges);
```

旧实现中八个方向分别计算 `QRect`、检查最小尺寸并调用 `setGeometry()` 的代码已全部删除。最小尺寸约束现在由 Qt 和 Windows 共同处理。

### 光标

鼠标没有按下时，`updateResizeCursor()` 根据边缘组合显示水平、垂直或对角缩放光标。最大化状态下不会返回缩放边缘，也不会显示缩放光标。

## 三、窗口状态

原来的最大化会缓存启动屏幕尺寸，然后手动将窗口移动到 `(0, 0)`。这在副屏、非零屏幕原点和任务栏位于其他方向时不正确。

现在最大化按钮使用标准切换：

```cpp
isMaximized() ? showNormal() : showMaximized();
```

最小化直接使用 `showMinimized()`，删除了同时修改几何和透明度的自定义动画。这样窗口还原时不会再依赖手动保存的矩形。

最大化时，`changeEvent()` 会把 6 像素的边缘外边距设为 0，并暂时取消圆角；还原时恢复边缘外边距和圆角。

以下旧状态已经不再需要：

- 启动时缓存的 `QScreen` 和 `availableGeometry()`。
- `primaryRect`、`beforeMaxiedRect` 和 `lastRect`。
- `maxied`、`bIsLeftPressed` 和 `location`。
- 鼠标偏移量以及手动 `setGeometry()`。
- 根据启动屏幕设置的窗口最大尺寸。

## 四、探索页缩放优化

修改文件：

- `src/ui/pages/explore/explorewidget.h`
- `src/ui/pages/explore/explorewidget.cpp`
- `src/ui/pages/explore/musicitemwidget.h`
- `src/ui/pages/explore/musicitemwidget.cpp`

### 复用歌曲卡片

原来的 `rebuildGridLayout()` 在列数变化时会把旧卡片移出布局，然后重新创建全部 `MusicItemWidget`。这会遗留旧控件，并让新控件再次请求封面。

现在使用 `currentRankSongWidgets` 保存当前卡片。列数变化时，只从网格中取出布局项，再把同一批卡片按新的行列位置加回去。

切换榜单数据时，`clearRankWidgets()` 会先删除旧卡片，再删除其对应的 `MusicItem`，避免卡片临时持有已经失效的数据指针。

### 合并封面缩放

原来的 `MusicItemWidget::resizeEvent()` 会再次调用当前控件的 `resize()`，可能形成重复布局；同时每变化约 8 像素就进行一次 `SmoothTransformation`。

现在：

- 卡片尺寸完全交给父布局，不在 `resizeEvent()` 中再次修改自己。
- `resizeEvent()` 只记录期望的封面宽度。
- 使用 80ms 单次定时器合并连续尺寸变化。
- 用户停止或放慢拖动后，再执行一次高质量封面缩放。

延迟由下面的代码控制：

```cpp
coverResizeTimer->setInterval(80);
```

如果希望封面更快跟随，可以改为 50–60ms；如果歌曲数量很多并且缩放仍有压力，可以提高到 100–120ms。

## 五、圆角与透明背景

本次保留了 `Qt::WA_TranslucentBackground` 和原有圆角绘制，避免改变当前视觉效果。

如果之后确认移动已经流畅，但连续改变窗口尺寸时仍然有明显重绘压力，可以单独进行 Windows 平台优化：使用 DWM 原生圆角并移除透明顶层窗口。这个改动涉及 Windows API 和 Windows 10/11 的兼容策略，建议作为独立改造，不与本次窗口交互逻辑混在一起。

## 六、验证清单

在 Qt Creator 中启动程序后，依次检查：

1. 从标题栏空白区域拖动窗口，按钮区域不应触发移动。
2. 将窗口快速拖到屏幕左右边缘，确认系统吸附行为正常。
3. 分别拖动四条边和四个角，确认光标方向和缩放方向一致。
4. 缩小到 `MainWidget` 的最小尺寸 `800 x 600`，窗口不应跳动或翻转。
5. 点击最大化按钮，再次点击应还原；双击标题栏也应切换状态。
6. 最大化时窗口外侧不应保留 6 像素空隙，也不应显示缩放光标。
7. 如果有多块显示器，将窗口移动到副屏后重复最大化、还原和边缘吸附。
8. 在探索页来回跨过一列/两列/三列阈值，歌曲卡片不应重复请求封面或不断增加对象。

## 七、构建说明

项目仍使用原来的 qmake 构建方式，不需要新增模块：

```text
qmake ../muzit.pro
mingw32-make
```

本次涉及的源文件已经通过 Qt 6.5.3 MinGW 编译阶段。当前完整命令在最终链接时仍需要项目原有的 FFmpeg 链接库配置；该链接问题与本次无边框窗口代码无关。
