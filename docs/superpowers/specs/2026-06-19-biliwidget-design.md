# BiliWidget 设计文档

**日期：** 2026-06-19  
**状态：** 已批准，待实现

---

## 目标

在 MainWidget 侧边栏新增"哔哩哔哩"入口，对应一个 `BiliWidget` 页面，允许用户通过关键词或 BV 号搜索 Bilibili 视频，配置下载参数后将音频保存到本地，并支持收藏操作。

---

## 整体布局

```
┌──────────────────────────────────────────────────────────┐
│  [关键词|BV号]  [──────── 搜索框 ────────]  [搜索]  每页[10▼]条 │  ← 搜索栏
├──────────────────────────────────┬───────────────────────┤
│  搜索结果列表（可滚动）              │  右侧面板              │
│  ┌──────────────────────────┐    │  ─ 保存设置 ─          │
│  │ 封面 | 标题 | UP主 | 时长 | ⬇ │    │    路径 / 文件名模板   │
│  ├──────────────────────────┤    │    音质选择             │
│  │ 封面 | 标题（选中，展开）    │    │  ─ 收藏 ─             │
│  │  ┌大封面 | BV信息 | 操作┐  │    │    添加到收藏           │
│  │  └──────────────────────┘│    │  ─ 下载队列 ─          │
│  ├──────────────────────────┤    │    进度条 × N           │
│  │ 封面 | 标题 | UP主 | 时长 | ⬇ │    │                       │
│  └──────────────────────────┘    │                       │
│  [◀]  1  2  3  [▶]              │                       │
└──────────────────────────────────┴───────────────────────┘
```

---

## 组件划分

### 1. `BiliWidget`（顶层，`QWidget`）

- 持有 `QNetworkAccessManager`（全局共享）
- 负责组合搜索栏、结果列表、右侧面板三块子组件
- 布局：`QVBoxLayout`（搜索栏在顶） + `QHBoxLayout`（结果列表 + 右侧面板）

### 2. `BiliSearchBar`（`QWidget`）

控件：
- `QButtonGroup` 切换搜索类型（关键词 / BV号），两个 `QPushButton` 做 toggle
- `QLineEdit` 输入框；按 Enter 触发搜索
- `QPushButton` 搜索按钮
- `QComboBox` 每页条数（10 / 20 / 30）

信号：`void searchRequested(QString keyword, SearchType type, int pageSize)`

### 3. `BiliResultList`（`QWidget`）

- `QScrollArea` 包裹 `QVBoxLayout`，容纳若干 `BiliResultItem`
- 翻页控件（`QPushButton` × 5：上一页、页码 × 3、下一页）放在列表底部
- 维护当前展开项（同时只展开一项）；展开/折叠通过 `BiliResultItem` 的信号驱动

信号：`void itemSelected(BiliVideoInfo info)`  
信号：`void downloadRequested(BiliVideoInfo info)`  
信号：`void favoriteRequested(BiliVideoInfo info)`

### 4. `BiliResultItem`（`QWidget`）

折叠态：封面缩略图（54×30）+ 标题/UP主/时长 + "⬇ 下载"快捷按钮  
展开态（折叠态下方追加）：封面大图（80×50）+ BV号/时长/分P数/简介 + "加入下载"按钮 + "★ 收藏"按钮

展开/折叠动画：`QPropertyAnimation` 改变高度（与 `FramelessWidget` 中最小化动画保持一致的做法）

### 5. `BiliSidePanel`（`QWidget`，固定宽度 200px）

三个区块，用 `QVBoxLayout` 堆叠，区块间用 `QFrame` 横线分隔：

**保存设置区**
- 路径行：`QLabel` 显示当前路径（截断） + `QPushButton "…"` 打开 `QFileDialog`
- 文件名模板：`QLineEdit`，默认 `{title} - {author}`
- 音质：`QComboBox`（320kbps / 192kbps / Hi-Res），对应结构体值 `"320kbps"` / `"192kbps"` / `"hires"`

**收藏区**
- `QLabel` 显示当前选中歌曲名（未选中时显示提示文字）
- `QPushButton "★ 添加到收藏"`

**下载队列区**
- `QScrollArea` 包裹若干下载进度行（`QLabel` + `QProgressBar`）
- 每行对应一个下载任务；下载完成后自动移除或变为"已完成"状态

信号：`void saveSettingsChanged(BiliSaveSettings settings)`

### 6. 数据结构

```cpp
struct BiliVideoInfo {
    QString bvId;
    QString title;
    QString author;
    QString coverUrl;
    int     duration;   // 秒
    int     partCount;
    QString description;
    int     playCount;
};

struct BiliSaveSettings {
    QString savePath;
    QString fileNameTemplate;  // 支持 {title} {author} {bvid}
    QString quality;           // "320kbps" / "192kbps" / "hires"
};
```

---

## 数据流

```
用户输入 → BiliSearchBar 发出 searchRequested
         → BiliWidget 调用 Bilibili 搜索 API
         → 解析 JSON → 构造 BiliVideoInfo 列表
         → BiliResultList 清空并重建 BiliResultItem 列表

用户点击行 → BiliResultItem 展开
           → BiliResultList 发出 itemSelected
           → BiliSidePanel 更新收藏区显示当前歌曲名

用户点"加入下载" → BiliWidget 读取 BiliSidePanel 当前 BiliSaveSettings
                → 发起下载任务
                → BiliSidePanel 下载队列新增一行，开始更新进度
```

---

## Bilibili API

搜索接口（关键词）：
```
GET https://api.bilibili.com/x/web-interface/search/type
    ?search_type=video&keyword=<keyword>&page=<page>&page_size=<size>
```

BV 号直接获取视频信息：
```
GET https://api.bilibili.com/x/web-interface/view?bvid=<bvid>
```

音频流下载需要先获取 cid，再请求 playurl，属于后续功能，本次只实现布局，下载逻辑暂为占位。

---

## 接入 MainWidget

1. 在 `mainwidget.h` 添加 `BiliWidget *biliWidget`
2. `initSidebar()` 增加 `biliButton`（"哔哩下载"），加入 `stackButtonGroup`，id = 2
3. `initRight()` 中 `funcWidget->addWidget(biliWidget)`
4. `muzit.pro` 新增 `biliwidget/` 子目录，仿照 `musicItem/` 用 `.pri` 引入

---

## 本次实现范围

- [x] `BiliWidget` 整体布局骨架
- [x] `BiliSearchBar` 完整控件
- [x] `BiliResultList` + `BiliResultItem` 布局（折叠/展开，无网络请求）
- [x] `BiliSidePanel` 保存设置 + 收藏区 + 下载队列占位
- [x] 接入 `MainWidget` 侧边栏
- [ ] 实际网络搜索（后续）
- [ ] 实际下载逻辑（后续）
