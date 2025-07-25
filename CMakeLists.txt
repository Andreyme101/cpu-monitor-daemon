cmake_minimum_required(VERSION 3.10)
project(cpu_monitor_daemon)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpu_monitor
    src/main.cpp
)

target_include_directories(cpu_monitor PRIVATE include)

