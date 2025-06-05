cmake_minimum_required(VERSION 3.10)
project(RemoteMonitor)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

find_package(libssh REQUIRED)

add_executable(remote_monitor
    src/main.cpp
    src/ssh_connection.cpp
    src/hardware_monitor.cpp
)

target_link_libraries(remote_monitor ssh)
