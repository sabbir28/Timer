# Changelog

All notable changes to the Timer420 project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.1] - 2026-10-25

### Added
- **2026 Era Fluid Animation Engine**:
  - Organic sinusoidal breathing curves (`animator_get_breathing`) for live ambient pulsing.
  - Travelling neon shimmer sweeps (`animator_get_shimmer_phase`) across active progress indicators.
  - Dynamic glow color synthesis (`animator_glow_color`) and smooth easing functions (`animator_ease_out_cubic`, `animator_ease_in_out_sine`).
  - Unit test suite coverage for 2026 animator curves and color interpolations (`tests/unit/test_animator.c`).
- **Modernized 2026 Dark Mode Admin Panel (Control Center)**:
  - Windows 11 / 2026 Immersive Dark Mode Titlebar (`DwmSetWindowAttribute` with `DWMWA_USE_IMMERSIVE_DARK_MODE`).
  - Deep Obsidian Slate theme styling (`#0F172A` background, `#182032` card containers, `#1E293B` input fields) with automatic GDI brush management.
  - Configurable **Break Duration** inputs (minutes and seconds) integrated into duration controls and settings persistence.
  - **1-Click Productivity Flow Templates**: Quick setup buttons for `🍅 25/5 Pomodoro`, `⚡ 50/10 Focus`, `🚀 90/15 Sprint`, and `🧘 2h/20m Flow`.
- **Rebuilt 2026 Visual Renderers**:
  - `renderer_progress.c`: Rounded obsidian pill track, dynamic glow bar, travelling neon photon shimmer sweep, and modern percentage badge.
  - `renderer_edge.c`: Cyber HUD corner bracket overlays, animated photon trail sweeping the screen boundary, and organic breathing borders.
  - `renderer_minimal.c`: Ambient glassmorphic breathing glow border, emerald break badge pill, and refined typographic hierarchy.

## [0.2.0] - 2026-10-25

### Added
- **Work / Break Alternating Session Engine**: Automatically switches between focused work intervals and break periods with distinct visual indicators and completion alerts.
- **Dedicated Work/Break Switch Button**: One-click session switcher (`☕ Work/Break`) in both the Control Center GUI and the System Tray context menu.
- **Live Sound Preview**: Added `🔊 Test Alarm Sound` button in Control Center and System Tray to preview custom `.wav` chimes immediately.
- **Custom Sound Path Persistence**: Full `%APPDATA%\Timer420\timer420.ini` serialization and recovery for user-selected audio files.
- **Automated Release Pipeline**: GitHub Actions CI/CD release workflow for multi-environment Windows x64 binaries, zip packaging, and release note generation.
- **Automated Release Script**: Added `scripts/release.ps1` for local packaging, tagging, and git server synchronization.

### Fixed
- Fixed missing `<stdint.h>` include in `src/ui/animation/animator.h` causing MSVC compilation failures.
- Fixed unused variable warning in `tests/unit/test_timer_engine.c` during NDEBUG optimized builds.
- Fixed git tracking of transient `build/` files, enforcing `.gitignore` rules.

## [0.1.0] - 2026-08-11

### Added
- Complete modular C11 refactoring of the prototype `LastMoment.c`.
- Monotonic high-resolution clock (`QueryPerformanceCounter`) eliminating wall-clock NTP time drift.
- Explicit timer state machine (`IDLE`, `RUNNING`, `PAUSED`, `COMPLETED`, `STOPPED`).
- Multi-cycle repeating timer engine with infinite `∞` cycle support.
- Windows System Power event listener (`WM_POWERBROADCAST`) for system sleep/resume auto-sync.
- Settings persistence in `%APPDATA%\Timer420\timer420.ini`.
- Windows Notification Toast and audible completion alerts (`WinMM`).
- Modular GDI Renderers for Minimal, Progress, and Edge Border overlay modes.
- `FakeClock` mock and unit test suite runnable in under 1ms.
- CMake build configuration supporting MSVC, GCC (MinGW), and Clang.
- GitHub Actions CI workflows (`build.yml`, `test.yml`).
