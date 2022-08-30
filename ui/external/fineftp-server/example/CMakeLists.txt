cmake_minimum_required(VERSION 3.5.1)

project(example)

set(CMAKE_CXX_STANDARD 14)

find_package(fineftp REQUIRED CONFIG)

set(sources
    src/main.cpp
)

add_executable (example
    ${sources}
)

target_link_libraries (example
    fineftp::server
)
