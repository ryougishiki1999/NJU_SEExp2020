cmake_minimum_required(VERSION 3.4.3)
project(HiSA)

# Set the ISO C++17 of Project
set(CMAKE_CXX_STANDARD 17)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# ATTENTION: DON'T MODIFY THIS FILE
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /GR-")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_subdirectory(core)
add_subdirectory(console)