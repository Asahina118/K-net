cmake_minimum_required(VERSION 3.20)

project(K-net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(K-net
    src/main.cpp
    examples/udp_echo/server.cpp
    src/endpoint.cpp
    src/udp/udp_socket.cpp
    src/detail/socket_handle.cpp
)

target_include_directories(K-net
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)
