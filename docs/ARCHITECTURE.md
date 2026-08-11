# Timer420 Architecture Guide

## System Components

Timer420 follows a modular, decoupled layered architecture separating core timing logic from Windows GUI and OS APIs.

```text
Timer420 Architecture Diagram

┌─────────────────────────────────────────────────────────────┐
│                      UI Presentation Layer                   │
│   (overlay_window.c, tray_manager.c, GDI Renderers)        │
└──────────────────────────────┬──────────────────────────────┘
                               │
┌──────────────────────────────▼──────────────────────────────┐
│                    Application Controller                   │
│                      (app_controller.c)                     │
└─────┬────────────────────────┬────────────────────────┬─────┘
      │                        │                        │
┌─────▼──────────────┐   ┌─────▼──────────────┐   ┌─────▼──────────────┐
│     Core Timer     │   │     Services       │   │  Platform Windows  │
│  (timer_engine.c)  │   │  (Audio, Toast,    │   │  (win32_power.c,   │
│  (timer_state.c)   │   │   Persistence)     │   │   win32_hotkey.c)  │
│  (monotonic_clock) │   │                    │   │                    │
└────────────────────┘   └────────────────────┘   └────────────────────┘
```

## Layer Descriptions

1. **`src/core/`**:
   - `clock/`: High-resolution clock interface (`Clock`) enabling production monotonic hardware clocks (`QueryPerformanceCounter`) and test mocks (`FakeClock`).
   - `timer/`: Core timer engine (`TimerEngine`) managing elapsed time calculations, repeat cycles, and event callbacks.
   - `state/`: Explicit state transition validator (`timer_state_can_transition`).
   - `config/`: Application settings model and sanitization rules.

2. **`src/services/`**:
   - `audio/`: Asynchronous sound notification engine.
   - `notification/`: Windows Toast notification builder.
   - `persistence/`: `%APPDATA%\Timer420\timer420.ini` parser and writer.
   - `logging/`: Structured multi-level logging.

3. **`src/platform/`**:
   - `windows/`: OS power broadcast listeners and global hotkey bindings.

4. **`src/ui/`**:
   - `renderers/`: Modular GDI paint routines for Minimal, Progress, and Edge Border overlay visual modes.
   - `tray/`: System tray icon lifecycle and popup context menu.
   - `windows/`: Layered transparent window setup (`WS_EX_LAYERED`, `LWA_COLORKEY`).
