---
layout: page
title: "Architecture Analysis"
permalink: /architecture-analysis.html
---

# Architecture Analysis - LastMoment / Timer420

## 1. Overview & Current Architecture

The prototype is implemented as a single C source file (`LastMoment.c`, ~576 lines) targeting 32-bit/64-bit Windows using pure native Win32 API, GDI, Shell API, and Common Controls.

### Core Components in Prototype:
- **Global Application State**: `static AppState g` holding OS window handles (`HWND`, `HINSTANCE`, fonts, tray menu), timer booleans (`running`, `isBreak`, `visible`), timing data (`totalSeconds`, `remainingSeconds`, `deadlineEpoch`), visual mode (`mode`), and opacity (`alpha`).
- **Timer Mechanism**: Wall-clock timestamp arithmetic (`time(NULL)` and `g.deadlineEpoch`).
- **UI & Graphics**: Double-buffered GDI rendering via a borderless layered window with `LWA_COLORKEY` (`RGB(0,0,0)` transparency).
- **User Input & OS Hooks**: Global Windows hotkeys (`RegisterHotKey`) and System Tray Notification Icon (`Shell_NotifyIconW`).

---

## 2. Current Application Flow

```mermaid
flowchart TD
    A[wWinMain Entry Point] --> B[InitCommonControlsEx]
    B --> C[CreateOverlayWindow]
    C --> D[Register Window Class 'LastMomentOverlay']
    C --> E[CreateWindowEx WS_EX_LAYERED | WS_EX_TOPMOST]
    C --> F[CreateFonts & CreateTrayIcon]
    C --> G[RegisterHotKey Toggle/StartPause/Reset/Esc]
    C --> H[SetSession DEFAULT_WORK_SECONDS]
    H --> I[Win32 Message Loop GetMessage]
    I --> J{Message Type}
    J -->|WM_TIMER 250ms| K[Update remainingSeconds from deadlineEpoch]
    J -->|WM_PAINT| L[PaintOverlay GDI Double Buffer]
    J -->|WM_HOTKEY| M[Handle Global Hotkeys]
    J -->|WM_TRAYICON / WM_COMMAND| N[Handle Tray Context Menu]
    J -->|WM_DESTROY| O[Cleanup GDI, Hotkeys, Tray & Exit]
```

---

## 3. Current Timer Engine & Behavior

1. **Time Tracking**:
   - `deadlineEpoch = time(NULL) + remainingSeconds`.
   - On `WM_TIMER` (fired every 250ms), calculates `remaining = deadlineEpoch - time(NULL)`.
2. **Pause / Resume**:
   - **Pause**: Calculates remaining seconds at freeze time: `remainingSeconds = deadlineEpoch - time(NULL)`.
   - **Resume**: Re-calculates new deadline: `deadlineEpoch = time(NULL) + remainingSeconds`.
3. **Accuracy & Limitations**:
   - Relies on `time(NULL)` (standard C library wall-clock time). Resolution is 1 second.
   - Vulnerable to Windows system clock adjustments, time zone changes, and NTP synchronization.
   - No sub-second precision.
   - Timer stops at 0 with no repetition or cycle tracking.

---

## 4. Current Dependencies

- **C Standard Library**: `stdio.h`, `time.h`, `math.h`
- **Windows APIs**:
  - `user32.lib`: Window creation, message pump, layered window attributes, global hotkeys.
  - `gdi32.lib`: Font creation, BitBlt double-buffering, pens, brushes, color keying.
  - `shell32.lib`: System tray icon integration (`Shell_NotifyIconW`).
  - `comctl32.lib`: Common controls initialization (`InitCommonControlsEx`).

---

## 5. Architectural Problems & Technical Debt

1. **Monolithic Single-File Design**: Timer math, UI rendering, event dispatching, Win32 API calls, and state management are tightly coupled in `LastMoment.c`.
2. **Global Mutable State**: `static AppState g` makes the application impossible to unit test without instantiating full Windows handles and GUI objects.
3. **Global Escape Hotkey Intercept Bug**: `RegisterHotKey(hwnd, HOTKEY_ESC, 0, VK_ESCAPE)` registers `VK_ESCAPE` as a global hotkey across the entire OS. This intercepts the Escape key in all other Windows applications while Timer420 is running.
4. **Lack of Persistence**: Timer duration, active mode, and window settings are lost on application restart.
5. **No Sound or Toast Notifications**: When the timer expires, the overlay only changes text color to red ("TIME'S UP") with no audible alert or Windows desktop notification.
6. **No Monotonic High-Resolution Clock**: Wall-clock time can drift or jump when system time changes.
7. **Resource Allocation in GDI Paint Loop**: `PaintOverlay` creates and destroys GDI pens, brushes, memory DCs, and bitmaps every paint frame (every 250ms).

---

## 6. Migration Risks & Mitigation Strategy

| Risk | Impact | Mitigation Strategy |
| :--- | :--- | :--- |
| **Breaking Transparent Visual Overlay** | High | Extract GDI rendering logic into isolated render modules while preserving Win32 layered window setup (`LWA_COLORKEY`). |
| **Timer Precision Regression** | High | Introduce monotonic clock interface (`QueryPerformanceCounter` / `GetTickCount64`) wrapped behind a `Clock` abstraction. |
| **Global Hotkey Conflicts** | Medium | Make global hotkeys configurable, remove unmodified ESC global hotkey, and add window-level key filters. |
| **Cross-Thread UI / Timer Race Conditions** | Medium | Ensure timer engine operates deterministically on a single control thread with clean event queues. |

---

## 7. Features Preserved from Original Prototype

1. **Fullscreen Transparent Overlay**: Topmost layered window displaying overlay text/graphics without obscuring underlying desktop workflow.
2. **3 Display Modes**:
   - `MODE_MINIMAL`: Clean time display with deadline indicator.
   - `MODE_PROGRESS`: Minimal mode plus progress bar and remaining percentage.
   - `MODE_EDGE`: 6px perimeter progress indicator tracing the edge of the screen.
3. **Color-Coded Status Guidance**:
   - Normal: Light Blue (`RGB(220, 230, 255)`).
   - Last 10 Minutes: Warning Orange (`RGB(255, 180, 40)`).
   - Completed: Red (`RGB(255, 60, 60)`).
   - Break Session: Green (`RGB(100, 200, 140)`).
4. **System Tray Navigation**: Tray icon with right-click menu (Preset session lengths: 2h, 1h, 30m, 15m; Overlay mode switcher; Start/Pause/Reset controls).
5. **Global Hotkeys**: `Ctrl+Shift+Space` (Toggle visibility), `Ctrl+Shift+S` (Start/Pause), `Ctrl+Shift+R` (Reset).

---

## 8. Target Architecture Proposal

```text
Timer420/
├── src/
│   ├── core/                  # Core application engine (Zero Win32 UI dependencies)
│   │   ├── clock/             # High-resolution monotonic time source (MonotonicClock, FakeClock)
│   │   ├── timer/             # Timer engine, state machine, duration, cycle manager
│   │   ├── state/             # Active state model and transition validation
│   │   └── config/            # Settings model, default configurations, serialization
│   │
│   ├── platform/              # OS-specific abstraction layer
│   │   └── windows/           # Windows sleep/resume listeners, hotkey manager, Win32 platform glue
│   │
│   ├── services/              # Infrastructure services
│   │   ├── audio/             # Sound notification engine (WinMM / PlaySound)
│   │   ├── notification/      # Windows Toast / Tray notifications
│   │   ├── persistence/       # INI file settings storage
│   │   └── logging/           # Lightweight file/console logger with log levels
│   │
│   ├── app/                   # Application orchestration layer
│   │   ├── controller/        # App controller coordinating Timer, Services, and UI
│   │   └── events/            # Application event definitions (TimerStarted, TimerCompleted, etc.)
│   │
│   └── ui/                    # Presentation layer
│       ├── windows/           # Win32 Window creation, message loop, Layered Window manager
│       ├── tray/              # System tray icon & context menu controller
│       └── renderers/         # GDI Renderers (Minimal, Progress, Edge Border modes)
│
├── tests/                     # Unit and Integration test suite
│   ├── unit/                  # Timer engine, state machine, clock, config tests
│   ├── integration/           # Controller & event flow tests
│   └── mock/                  # FakeClock, MockServices
│
├── docs/                      # Technical & project documentation
├── assets/                    # Icons, sound effects, resources
├── scripts/                   # Build, test, and release helper scripts
└── CMakeLists.txt             # Modern CMake build system configuration
```
