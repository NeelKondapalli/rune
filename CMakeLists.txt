cmake_minimum_required(VERSION 3.20)
project(rune LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)

# ---- Library (core engine) ----
add_library(rune
    src/converter.cpp
    src/writer.cpp
)

target_include_directories(rune
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party
)

target_link_libraries(rune
    PRIVATE ZLIB::ZLIB
)

# ---- CLI executable ----
add_executable(rune_cli
    apps/rune_cli.cpp
)

target_link_libraries(rune_cli
    PRIVATE rune
)
