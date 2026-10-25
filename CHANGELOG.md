# Changelog

All notable changes to the Timer420 project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
