# Muzit 项目结构现状与演进规划

最后更新：2026-09-13

## 1. 文档目的

本文记录 Muzit 当前完成的目录重构、各模块职责、仍然存在的耦合，以及未来支持更多音视频平台和歌曲元信息来源时的演进方向。

当前重构以“不改变现有业务行为”为原则，先完成文件归位和模块边界划分，再随着功能落地逐步提取接口和服务，避免提前创建大量空模块。

## 2. 当前项目结构

```text
muzit/
├── src/
│   ├── app/
│   │   ├── app.pri
│   │   ├── main.cpp
│   │   ├── mainwidget.h/.cpp
│   │   └── config.h/.cpp
│   │
│   ├── core/
│   │   ├── audio/
│   │   │   ├── audio.pri
│   │   │   ├── audiotypes.h
│   │   │   ├── audioconverter.h/.cpp
│   │   │   ├── audioprocessor.h/.cpp
│   │   │   ├── audioconverttask.h/.cpp
│   │   │   ├── audioconvertmanager.h/.cpp
│   │   │   ├── m4aremuxer.h/.cpp
│   │   │   ├── mp3transcoder.h/.cpp
│   │   │   └── ffmpegutils.h/.cpp
│   │   │
│   │   ├── download/
│   │   │   ├── download.pri
│   │   │   ├── downloadtask.h/.cpp
│   │   │   └── downloadmanager.h/.cpp
│   │   │
│   │   └── metadata/
│   │       ├── metadata.pri
│   │       └── metadatatypes.h
│   │
│   ├── platforms/
│   │   └── bilibili/
│   │       ├── bilibili.pri
│   │       ├── bilitypes.h
│   │       ├── bilidltool.h/.cpp
│   │       └── api/
│   │           ├── bilibiliapi.h/.cpp
│   │           ├── bilibiliclient.h/.cpp
│   │           └── apiparser.h/.cpp
│   │
│   └── ui/
│       ├── ui.pri
│       ├── uicommon.h
│       ├── window/
│       │   ├── framelesswidget.h/.cpp
│       │   └── titlebar.h/.cpp
│       ├── widgets/
│       │   └── playerwidget.h/.cpp
│       └── pages/
│           ├── explore/
│           │   ├── explorewidget.h/.cpp
│           │   ├── musicitem.h/.cpp
│           │   └── musicitemwidget.h/.cpp
│           ├── local/
│           │   └── localwidget.h/.cpp
│           └── download/
│               ├── downloadwidget.h/.cpp
│               └── bilibili/
│                   ├── bilidlwidget.h/.cpp
│                   ├── bilisearchbar.h/.cpp
│                   ├── biliresultlist.h/.cpp
│                   ├── biliresultitem.h/.cpp
│                   ├── bilisidepanel.h/.cpp
│                   └── biliaudiometadatadialog.h/.cpp
│
├── resources/
├── tests/
├── docs/
├── resources.qrc
└── muzit.pro
```

## 3. 当前模块职责

### `src/app`

负责程序入口、全局配置和主窗口组装。该模块可以依赖核心功能和 UI，但不应该承载具体平台的请求解析逻辑。

### `src/core/audio`

负责通用音频处理，包括格式选择、转码任务、M4A 封装和 MP3 编码。该模块不应了解 Bilibili、iTunes 或具体页面。

### `src/core/download`

负责通用网络文件下载、并发队列和下载任务生命周期。它只处理请求与文件，不决定歌曲来自哪个平台。

### `src/core/metadata`

目前只保存通用的 `AudioMetadata` 数据类型。未来将在这里定义元信息提供者接口、候选结果和匹配逻辑。

### `src/platforms/bilibili`

负责 Bilibili 特有的数据结构、接口地址、请求签名、响应解析以及音频下载任务的组织。其他平台不应依赖这里的类型。

### `src/ui`

负责窗口、页面和可视控件。下载页下的 `bilibili` 子目录表示这些控件当前仍然依赖 Bilibili 数据类型，并不是跨平台通用控件。

## 4. 页面与功能的分离现状

当前已经完成目录层面和部分职责层面的分离：

- 页面与控件位于 `ui`。
- 音频处理和通用下载位于 `core`。
- Bilibili 接口与平台数据位于 `platforms/bilibili`。
- `core` 和平台层不再依赖原先大而全的 `common.h`。

目前还不是完全解耦，主要问题包括：

- `BiliDLWidget` 同时负责用户交互、搜索流程、分 P 加载和下载流程调度。
- `ExploreWidget` 仍然直接调用 Bilibili API。
- `BiliSidePanel` 直接访问应用配置。
- `BiliDLTool` 同时承担平台请求、文件名生成、下载启动和转码调度。
- `bilitypes.h` 同时包含平台响应模型、保存设置和下载任务模型。
- `uicommon.h` 仍然包含较多 Qt 头文件及字体、布局、缓存路径等不同职责。

因此，当前状态可以概括为：

```text
目录边界已经建立
核心能力基本独立
部分页面仍负责业务流程编排
平台模型还需要继续细分
```

## 5. 目标结构

随着元信息查询和更多视频平台接入，目标结构计划扩展为：

```text
src/
├── app/
├── core/
│   ├── audio/
│   ├── download/
│   ├── files/
│   └── metadata/
│       ├── metadatatypes.h
│       ├── metadataprovider.h
│       └── metadatamatcher.h/.cpp
│
├── platforms/
│   ├── bilibili/
│   │   ├── api/
│   │   ├── models/
│   │   ├── services/
│   │   └── bilimetadatatitleparser.h/.cpp
│   ├── itunes/
│   │   └── itunesmetadataprovider.h/.cpp
│   └── other-platform/
│       ├── api/
│       ├── models/
│       └── services/
│
└── ui/
    ├── window/
    ├── widgets/
    ├── dialogs/
    │   └── audiometadatadialog.h/.cpp
    └── pages/
```

计划中的目录不需要提前全部创建，应在对应功能实现时加入。

## 6. 当前结构与目标结构的差异

### 元信息模块

当前只有 `metadatatypes.h`。实现元信息搜索时，再增加：

- `MetadataProvider`：统一不同数据来源的查询接口。
- `MetadataCandidate`：表示一条候选歌曲信息及其来源。
- `MetadataMatcher`：根据歌名、歌手和时长计算候选匹配度。
- `ITunesMetadataProvider`：调用 iTunes 接口并转换结果。

### 标题解析

当前尚未创建标题解析器。由于 Bilibili 标题包含平台特有的命名习惯，计划放置在：

```text
src/platforms/bilibili/bilimetadatatitleparser.h/.cpp
```

它只负责清理标题、提取初步歌名和歌手、生成搜索关键词，不负责网络请求和界面展示。

### 元信息 Dialog

当前 `BiliAudioMetadataDialog` 直接接收 `BiliVideoInfo`，因此仍属于 Bilibili 下载页面。

当它改为只接收通用的歌曲编辑项和元信息候选后，可以重命名并迁移到：

```text
src/ui/dialogs/audiometadatadialog.h/.cpp
```

### 探索页面模型

`MusicItem` 和 `MusicItemWidget` 目前仅供探索页面使用，所以暂时一起放在 `ui/pages/explore`。如果以后播放器、本地音乐页等模块也需要 `MusicItem`，再将数据模型移动到核心模型目录，仅保留 Widget 在 UI 层。

### UI 公共内容

原来的 `common.h` 已收敛为 `ui/uicommon.h`，但仍是过渡方案。后续可按实际复用情况拆成：

```text
core/files/apppaths.h
ui/layoututils.h
ui/theme/fontmanager.h/.cpp
```

## 7. 依赖规则

建议保持以下依赖方向：

```text
app
 ├──> ui
 ├──> platforms
 └──> core

ui
 ├──> platforms
 └──> core

platforms
 └──> core

core
 └──> Qt/FFmpeg 等基础库
```

应避免：

- `core` 引用页面或具体平台类型。
- Bilibili 模块引用另一个平台的实现。
- 网络 Client 直接操作 Widget。
- Widget 直接解析平台 JSON。
- 把所有辅助函数继续放入一个全局 `utils` 文件。

## 8. 工具和辅助代码的放置规则

辅助代码应尽量靠近其所属功能：

| 功能 | 推荐位置 |
|---|---|
| FFmpeg 错误处理、FFmpeg 通用操作 | `core/audio/ffmpegutils` |
| Bilibili 标题清理 | `platforms/bilibili/bilimetadatatitleparser` |
| iTunes 元信息查询 | `platforms/itunes/itunesmetadataprovider` |
| 文件名非法字符处理 | `core/files/filenameutils` |
| 缓存目录和应用路径 | `core/files/apppaths` |
| 响应式页面列数 | `ui/layoututils` |
| 字体和主题加载 | `ui/theme` |

判断原则：

1. 只被一个模块使用时，放在该模块内部。
2. 与特定外部平台相关时，放在对应 `platforms` 目录。
3. 只操作界面时，放在 `ui`。
4. 与业务无关并被多个模块复用时，才放入通用核心目录。
5. 文件名应体现具体职责，避免不断扩张的 `utils.cpp`。

## 9. 后续演进步骤

### 阶段一：完成歌曲元信息查询

1. 定义通用的元信息候选和 Provider 接口。
2. 实现 Bilibili 标题清理与初步提取。
3. 实现 iTunes 查询 Provider。
4. 根据歌名、歌手和时长进行候选排序。
5. 在 Dialog 中展示候选，并允许用户手动修改。

### 阶段二：写入音频元信息

1. 将最终选择的 `AudioMetadata` 传入转换选项或转换任务。
2. 在 FFmpeg 输出文件头写入前设置通用文本元信息。
3. 分别验证 MP3、M4A 和 FLAC 的播放器兼容性。
4. 最后单独实现不同容器的封面写入。

### 阶段三：降低页面业务耦合

1. 从 `BiliDLWidget` 提取搜索与下载流程控制服务。
2. 拆分 `BiliDLTool` 中的文件名生成和转码调度职责。
3. 让页面主要负责显示状态、转发用户操作和展示结果。
4. 评估是否将探索页的数据来源抽象为统一平台接口。

### 阶段四：接入其他平台

1. 在 `platforms` 下新增独立目录。
2. 平台内部定义自己的 API、响应模型和下载服务。
3. 尽量复用 `core/download`、`core/audio` 和 `core/metadata`。
4. 只有在多个平台出现真实共性后，才提取新的通用接口。

## 10. 构建状态

重构后主工程和测试工程均可通过 qmake 配置，所有项目源文件可以成功编译。

当前命令行环境的最终链接失败，是因为命令行使用 MinGW，而 `audio.pri` 配置的是 vcpkg `x64-windows` 的 MSVC FFmpeg 库。项目原定构建环境仍为 Qt 6.5.3 MSVC2019 64 位；应在该 Qt Creator Kit 下完成最终链接验证。

该工具链问题在重构前已经存在，与本次目录和依赖调整无关。
