cmake_minimum_required(VERSION 3.20)

project(K-net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(knet
    src/endpoint.cpp
    src/udp/udp_socket.cpp
    src/detail/socket_handle.cpp

    src/reliable_udp/packet.cpp
    src/reliable_udp/reliable_udp_socket.cpp
)

# includes the example main.cpp and server.cpp entry point
add_executable(udp_echo_server
    src/main.cpp
    examples/udp_echo/server.cpp
)

target_include_directories(knet PUBLIC
        ${PROJECT_SOURCE_DIR}
)

target_link_libraries(udp_echo_server PRIVATE knet)


enable_testing()
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests test)
endif()

