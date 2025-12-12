cmake_minimum_required(VERSION 3.16)
project(xsmall_hmi_editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Скачивание и сборка SFML 3.0.2 из GitHub
include(FetchContent)

message(STATUS "Downloading SFML 3.0.2...")
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
    GIT_SHALLOW    TRUE
)

# Настройка SFML (отключаем не нужные модули для ускорения сборки)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio module" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML network module" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "Build SFML documentation" FORCE)

FetchContent_MakeAvailable(sfml)

# Скачивание Google Test
message(STATUS "Downloading Google Test...")
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)

# Настройка GTest
set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT for GTest" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Disable GMock" FORCE)

FetchContent_MakeAvailable(googletest)

# Основное приложение
add_executable(xsmall_hmi_editor
    src/main.cpp
    src/VisualObject.cpp
    src/VariableDatabase.cpp
    src/Editor.cpp
    src/Palette.cpp
)

# Подключаем SFML к основному приложению
target_link_libraries(xsmall_hmi_editor
    sfml-graphics
    sfml-window
    sfml-system
)

# Тесты
add_executable(xsmall_hmi_editor_tests
    src/test_main.cpp
    src/VariableDatabase.cpp
    src/VisualObject.cpp
    src/Palette.cpp
)

# Подключаем GTest и SFML к тестам
target_link_libraries(xsmall_hmi_editor_tests
    GTest::gtest_main
    sfml-graphics
    sfml-window
    sfml-system
)

# Копирование DLL файлов для Windows (если SFML собран динамически)
if(WIN32)
    add_custom_command(TARGET xsmall_hmi_editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:xsmall_hmi_editor>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:xsmall_hmi_editor>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:xsmall_hmi_editor>
    )
    
    add_custom_command(TARGET xsmall_hmi_editor_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:xsmall_hmi_editor_tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:xsmall_hmi_editor_tests>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:xsmall_hmi_editor_tests>
    )
endif()

message(STATUS "========================================")
message(STATUS "Build configuration:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  SFML Version: 3.0.2 (from GitHub)")
message(STATUS "  GTest Version: 1.14.0 (from GitHub)")
message(STATUS "========================================")