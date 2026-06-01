cmake_minimum_required(VERSION 3.28)
project(bean_eater)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML COMPONENTS Graphics Audio REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(bean_eater WIN32
    src/main.cpp
    src/game.h
    src/game.cpp)

target_link_libraries(bean_eater SFML::Graphics SFML::Audio)

# 打包

install(TARGETS bean_eater
    RUNTIME_DEPENDENCY_SET rutime_deps
    RUNTIME DESTINATION .
)
install(RUNTIME_DEPENDENCY_SET rutime_deps
    PRE_EXCLUDE_REGEXES
        "api-ms-.*" "ext-ms-.*"
        "advapi32\\.dll"
        "combase\\.dll"
        "DWrite\\.dll"
        "gdi32\\.dll"
        "glu32\\.dll"
        "kernel32\\.dll"
        "KernelBase\\.dll"
        "ntdll\\.dll"
        "ole32\\.dll"
        "opengl32\\.dll"
        "rpcrt4\\.dll"
        "sechost\\.dll"
        "shell32\\.dll"
        "user32\\.dll"
        "usp10\\.dll"
        "win32u\\.dll"
        "winmm\\.dll"
        "ws2_32\\.dll"
        # 微软运行时库（系统已有，或应由 vc_redist 提供）
        "msvcrt\\.dll" "msvcp.*\\.dll"
    DIRECTORIES "E:/Scoop/apps/msys2/current/clang64/bin"
    RUNTIME DESTINATION .
)
install(DIRECTORY assets/ DESTINATION assets)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "0.0.2")
set(CPACK_GENERATOR "ZIP")
include(CPack)