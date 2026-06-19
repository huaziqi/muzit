# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Muzit is a Qt6 desktop music player application (C++17) that aggregates Bilibili music charts. It uses a custom frameless window with drag-resize support and fetches chart data from the Bilibili API.

## Build

This project uses Qt Creator with qmake. Build via Qt Creator (Desktop Qt 6.5.3 MSVC2019 64bit) or from command line:

```
# From build directory
qmake ../muzit.pro
nmake          # or mingw32-make depending on toolchain
```

There are no tests and no lint commands configured.

## Architecture

**Widget hierarchy:**
- `MainWidget` (root, extends `FramelessWidget`) — holds the sidebar nav and right content area
  - `FramelessWidget` — custom borderless window base class; handles drag-to-move, edge-resize, minimize animation, and rounded-corner painting. All main windows should extend this.
  - `TitleBar` — custom title bar with close/min/max buttons; emits `buttonEvent(QString)` signal handled by `FramelessWidget::titleBarEvent()`
  - `ExploreWidget` — "探索音乐" tab; fetches Bilibili weekly music chart via `QNetworkAccessManager`, displays songs in a responsive `QGridLayout` (1–3 columns based on `common::getColumn()`)
  - `LocalWidget` — "本地音乐" tab; stub, not yet implemented
  - `PlayerWidget` — fixed bottom bar; stub for playback controls

**Data model (`musicItem/` subdirectory, included via `.pri`):**
- `MusicItem` (`QObject`) — plain data object: BVId, title, author, coverUrl, playedNum, duration, rank
- `MusicItemWidget` — display widget for one song card; downloads cover image into `QStandardPaths::CacheLocation/muzit/cover/` (filename = MD5 of URL), draws duration overlay onto the pixmap

**`common.h`** is a shared header included everywhere. It centralizes all Qt includes and provides:
- `common::cachePath` — writable cache directory
- `common::getColumn(width)` — responsive column count (1/2/3)
- `common::loadFont()` / `common::vonwaoFont` — pixel font (VonwaonBitmap)

**`Config`** — reads/writes `AppConfigLocation/config.json` on startup; creates the file with defaults if missing.

## Key Design Patterns

**Frameless window resize:** `FramelessWidget` tracks cursor position relative to a 4px border (`PADDING`) to determine a `Location` enum and updates cursor shape and geometry accordingly during `mouseMoveEvent`.

**Responsive grid:** `ExploreWidget::resizeEvent` calls `common::getColumn()` and calls `rebuildGridLayout()` when the column count changes. `rebuildGridLayout()` tears down and rebuilds the `QGridLayout` from `currentRankSongs`.

**Network pattern:** All API calls use `QNetworkAccessManager::get()` with lambda-connected `finished` signals. Replies are deleted via `deleteLater()` after handling.

**Bilibili API endpoints used:**
- Weekly chart list: `https://api.bilibili.com/x/copyright-music-publicity/toplist/all_period?list_type=1`
- Songs for a period: `https://api.bilibili.com/x/copyright-music-publicity/toplist/music_list?list_id=<ID>`

## Adding a New Page

1. Create `YourWidget` extending `QWidget`, add `.h`/`.cpp` to `HEADERS`/`SOURCES` in `muzit.pro`
2. Add a nav button in `MainWidget::initSidebar()` and register it with `stackButtonGroup`
3. Instantiate and add to `funcWidget` (the `QStackedWidget`) in `MainWidget::initRight()`
