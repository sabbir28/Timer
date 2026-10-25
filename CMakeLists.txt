cmake_minimum_required(VERSION 3.20)

project(Timer420
    VERSION 0.2.0
    DESCRIPTION "Professional Win32 Work Timer Desktop Application - Release 2.0"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Platform check
if(NOT WIN32)
    message(FATAL_ERROR "Timer420 is a native Windows desktop application and requires Windows OS.")
endif()

# Compiler options and warnings
if(MSVC)
    add_compile_options(/W4 /utf-8)
    add_compile_definitions(
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        _WIN32_WINNT=0x0601
        _CRT_SECURE_NO_WARNINGS
    )
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_definitions(
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        _WIN32_WINNT=0x0601
    )
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Core & Service Source Files
set(CORE_SOURCES
    src/core/clock/monotonic_clock.c
    src/core/timer/timer_engine.c
    src/core/state/timer_state.c
    src/core/config/settings.c
)

set(SERVICE_SOURCES
    src/services/logging/logger.c
    src/services/persistence/settings_persistence.c
    src/services/audio/audio_service.c
    src/services/notification/toast_service.c
    src/services/net/time_sync.c
    src/services/color/color_adapter.c
)

set(PLATFORM_SOURCES
    src/platform/windows/win32_power.c
    src/platform/windows/win32_hotkey.c
)

set(UI_SOURCES
    src/ui/animation/animator.c
    src/ui/renderers/renderer_minimal.c
    src/ui/renderers/renderer_progress.c
    src/ui/renderers/renderer_edge.c
    src/ui/tray/tray_manager.c
    src/ui/windows/overlay_window.c
    src/ui/windows/controller_window.c
)

set(APP_SOURCES
    src/app/controller/app_controller.c
    src/app/main.c
)

# Executable target
add_executable(Timer420 WIN32
    ${CORE_SOURCES}
    ${SERVICE_SOURCES}
    ${PLATFORM_SOURCES}
    ${UI_SOURCES}
    ${APP_SOURCES}
)

target_link_libraries(Timer420 PRIVATE
    user32
    gdi32
    shell32
    comctl32
    comdlg32
    winmm
    winhttp
    dwmapi
)

if(MINGW OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(Timer420 PRIVATE -municode)
endif()

# Testing setup
enable_testing()

set(TEST_SOURCES
    tests/mock/fake_clock.c
    tests/unit/test_clock.c
    tests/unit/test_timer_engine.c
    tests/unit/test_timer_state.c
    tests/unit/test_settings.c
    tests/unit/test_animator.c
    tests/test_runner.c
)

add_executable(timer420_tests
    ${CORE_SOURCES}
    ${SERVICE_SOURCES}
    src/ui/animation/animator.c
    ${TEST_SOURCES}
)

target_link_libraries(timer420_tests PRIVATE
    winmm
    winhttp
    gdi32
)

add_test(NAME Timer420UnitTestSuite COMMAND timer420_tests)
