# Changelog

All notable changes to the Timer420 project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
