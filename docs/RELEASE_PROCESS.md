---
layout: page
title: "Release Process"
permalink: /release-process.html
---

# Release Process

This document details how to package and release Timer420.

## Pre-Release Checklist

1. Verify all unit tests pass (`ctest --test-dir build --output-on-failure`).
2. Update version number in `CMakeLists.txt` and `CHANGELOG.md`.
3. Build Release binaries (`cmake --build build --config Release`).
4. Execute `scripts/package.ps1` to generate `Timer420-vX.Y.Z-win64.zip`.
5. Create GitHub Release tag and attach packaged `.zip`.
