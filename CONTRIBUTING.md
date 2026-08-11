# Contributing to Timer420

Thank you for your interest in contributing to Timer420! We welcome bug reports, feature proposals, documentation improvements, and pull requests.

## Code of Conduct

All contributors are expected to adhere to our [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

### 1. Reporting Bugs
Check existing [GitHub Issues](https://github.com/Timer420/Timer420/issues) before opening a new report. Use the Bug Report template and provide:
- Clear description of the bug
- Step-by-step reproduction instructions
- Expected vs actual behavior
- Windows version and build environment

### 2. Suggesting Features
Submit a feature proposal using the Feature Request issue template detailing the use case, problem solved, and proposed design.

### 3. Pull Requests
1. Fork the repository and create a feature branch (`git checkout -b feature/amazing-feature`).
2. Follow the C11 coding standards (4-space indentation, clear variable names, comment public header interfaces).
3. Ensure code builds cleanly with CMake without warnings.
4. Add unit tests for new core functionality in `tests/unit/`.
5. Run unit tests (`ctest --test-dir build --output-on-failure`).
6. Commit using conventional commit messages (e.g. `feat: add timer preset`, `fix: resolve GDI pen leak`).
7. Open a Pull Request referencing the issue.

## Development Setup

See [`docs/BUILDING.md`](docs/BUILDING.md) and [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md) for complete instructions.
