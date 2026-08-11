# Migration Plan - Timer420 Codebase Modernization

This document outlines the step-by-step migration plan for transforming the `LastMoment.c` prototype into a modular, production-grade, open-source Windows application.

---

## Migration Strategy Principles

1. **Incremental Execution**: Each phase represents a self-contained transformation.
2. **Continuous Buildability**: The project MUST build cleanly at the completion of every phase.
3. **Zero Functionality Loss**: All existing features (overlay rendering, modes, tray icon, hotkeys) must remain functional throughout.
4. **Decoupled Architecture**: Timer core logic strictly separated from Win32 UI dependencies.

---

## Detailed Migration Phases

### Phase 0 — Repository Analysis & Baseline Documentation
- **Goal**: Analyze original `LastMoment.c` codebase and produce baseline architecture documentation.
- **Deliverables**:
  - `docs/ARCHITECTURE_ANALYSIS.md`
  - `docs/TIMER_BEHAVIOR.md`
  - `docs/MIGRATION_PLAN.md`

---

### Phase 1 — Project Foundation & Build System
- **Goal**: Establish target folder structure and CMake build system.
- **Deliverables**:
  - `CMakeLists.txt` configured for MSVC / GCC / Clang on Windows.
  - Source directory layout (`src/core`, `src/platform`, `src/services`, `src/ui`, `src/app`, `tests`).
  - Compiler warning flags enabled (`/W4` on MSVC, `-Wall -Wextra` on GCC/Clang).

---

### Phase 2 — High-Precision Clock & Core Timer Engine
- **Goal**: Extract timer math from Win32 GUI into a pure C engine using a monotonic clock.
- **Deliverables**:
  - `src/core/clock/clock.h` & `src/core/clock/monotonic_clock.c` (`QueryPerformanceCounter`).
  - `src/core/timer/timer_engine.h` & `src/core/timer/timer_engine.c`.
  - Durations supporting 5s up to 24 hours without floating-point precision loss.

---

### Phase 3 — State Machine & Repeat Controller
- **Goal**: Implement explicit timer state machine and multi-cycle repeat manager.
- **Deliverables**:
  - Explicit states: `IDLE`, `RUNNING`, `PAUSED`, `COMPLETED`.
  - Cycle manager tracking `current_cycle`, `completed_cycles`, `repeat_enabled`.
  - Application-level event dispatching (`TimerCompleted`, `TimerTick`).

---

### Phase 4 — Platform Layer & Win32 Integration
- **Goal**: Isolate Windows API OS interactions into `src/platform/windows`.
- **Deliverables**:
  - Power / Sleep listener (`WM_POWERBROADCAST` handler for sleep/resume sync).
  - Clean Global Hotkey Manager (removing unsafe ESC key intercept bug).
  - DPI awareness helper (`SetProcessDpiAwarenessContext`).

---

### Phase 5 — Settings Engine & Persistence
- **Goal**: Add file persistence for timer presets, overlay settings, and preferences.
- **Deliverables**:
  - `src/services/persistence/settings_manager.c`.
  - INI configuration format (`timer420.ini` stored in `%APPDATA%/Timer420`).
  - Safe defaults, corruption handling, and schema validation.

---

### Phase 6 — Service Layer (Audio, Notifications, Logging)
- **Goal**: Build independent services for completion alerts and application logging.
- **Deliverables**:
  - `src/services/audio/audio_service.c` (WinMM / PlaySound completion sound).
  - `src/services/notification/toast_service.c` (Windows notification toasts).
  - `src/services/logging/logger.c` (Structured multi-level logging: DEBUG, INFO, WARN, ERROR).

---

### Phase 7 — UI Separation & Presentation Renderers
- **Goal**: Separate Win32 window management from GDI rendering strategies.
- **Deliverables**:
  - `src/ui/windows/overlay_window.c` (Layered window controller).
  - `src/ui/tray/tray_manager.c` (System tray icon and context menu).
  - `src/ui/renderers/renderer_minimal.c`, `renderer_progress.c`, `renderer_edge.c`.
  - Reusable GDI brush/pen caching to eliminate per-frame allocations.

---

### Phase 8 — Comprehensive Testing Framework
- **Goal**: Implement fast unit and integration tests using a simulated clock.
- **Deliverables**:
  - `tests/mock/fake_clock.c` (Instant time advancement: simulate 2 hours in 1 millisecond).
  - Unit tests covering state transitions, pause stability, repeat cycles, persistence, and edge cases.
  - Integration tests for controller event flow.

---

### Phase 9 — Open Source Assets & GitHub Infrastructure
- **Goal**: Prepare the repository for open-source publication and collaboration.
- **Deliverables**:
  - `README.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `CHANGELOG.md`, `ROADMAP.md`, `LICENSE`.
  - `.github/workflows/build.yml` & `test.yml` GitHub Actions CI.
  - `.github/ISSUE_TEMPLATE/bug_report.md` & `feature_request.md`.
  - `.github/PULL_REQUEST_TEMPLATE.md`, `.github/CODEOWNERS`.

---

### Phase 10 — Packaging & Release
- **Goal**: Produce release binaries and installer packages.
- **Deliverables**:
  - Standalone executable build target.
  - Automated release artifact archiving script.
