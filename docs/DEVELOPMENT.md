# Development Guide

## Codebase Organization

- `src/core/`: Zero Win32 UI dependency C engine logic.
- `src/services/`: Infrastructure services (audio, toast notifications, logging, INI persistence).
- `src/platform/`: Win32 power broadcast listeners and global hotkey managers.
- `src/ui/`: GDI double-buffer renderers, overlay window procedure, and tray manager.
- `tests/`: Unit test suite and `FakeClock` mock.

## Coding Conventions

- **C Standard**: C11 (`-std=c11` / `/std:c11`).
- **Formatting**: 4-space indentation, clear prefixing (`timer_engine_*`, `settings_*`).
- **Memory Safety**: No uninitialized variables, safe array bounds checking (`wcscpy_s`, `swprintf`).
- **Win32 API**: Use Unicode variants exclusively (`W` suffix, e.g. `CreateWindowExW`, `DrawTextW`).
