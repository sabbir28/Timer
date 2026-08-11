# Building Timer420

This guide explains how to compile Timer420 on Windows using various compiler toolchains and CMake.

---

## Required Environment

- **OS**: Windows 7 / 8.1 / 10 / 11 (64-bit or 32-bit)
- **CMake**: Version 3.20 or newer
- **Compiler**:
  - MSVC (Visual Studio 2019 / 2022 with C/C++ workload)
  - MinGW-w64 (GCC 10+)
  - LLVM Clang-CL

---

## Build Commands

### 1. MSVC (Visual Studio Command Prompt)

```cmd
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### 2. MinGW-w64 (GCC)

```cmd
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### 3. Ninja Generator

```cmd
cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
ninja -C build
```

---

## Build Output

- **Main Executable**: `build/Release/Timer420.exe`
- **Test Executable**: `build/Release/timer420_tests.exe`
