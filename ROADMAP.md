# Timer420 Project Roadmap

This document outlines the development roadmap for Timer420.

---

## Phase 0 — Prototype (Completed)
- Single-file C prototype (`LastMoment.c`).

## Phase 1 — Foundation & Modular Core (Completed)
- Clean C11 modular folder structure (`src/core`, `src/platform`, `src/services`, `src/ui`, `src/app`).
- Monotonic clock engine (`QueryPerformanceCounter`).
- Explicit 5-state timer machine (`IDLE`, `RUNNING`, `PAUSED`, `COMPLETED`, `STOPPED`).
- Unit test suite (`FakeClock`) and CMake build system.

## Phase 2 — Production Application (Completed)
- Settings persistence (`timer420.ini`).
- Audio completion alerts (`WinMM`).
- Windows Notification Toast notifications (`Shell_NotifyIconW`).
- Structured logging service (`logger.c`).

## Phase 3 — Windows Integration (Completed)
- Power/Sleep listener (`WM_POWERBROADCAST`).
- Safe global hotkeys (without global Escape key trap).
- DPI awareness and GDI double-buffering.

## Phase 4 — Open Source Infrastructure (Completed)
- Comprehensive technical documentation (`docs/`).
- GitHub Actions CI workflows (`build.yml`, `test.yml`).
- Issue templates, PR templates, and LICENSE (MIT).

## Phase 5 — Advanced Features (Future Target Releases)

### Completed in v0.2.0 (2026 Update):
- [x] Customizable sound file picker in tray menu & Control Center.
- [x] Custom break duration timer (work/break alternating cycles).
- [x] Preset manager & custom duration persistence.
- [x] Live sound preview alert testing.
- [x] Automated GitHub Actions multi-environment release packaging.

### Planned for v1.0.0:
- [ ] Multi-monitor edge border overlay option.
- [ ] Statistics & daily productivity log exporter.
- [ ] Configurable global hotkey mapping editor.
