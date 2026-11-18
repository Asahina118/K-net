cmake_minimum_required(VERSION 3.20)

project(K-net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(knet
    src/endpoint.cpp
    src/udp/udp_socket.cpp
    src/reliable_udp/packet.cpp
    src/detail/socket_handle.cpp
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

add_library(doctest INTERFACE)

target_include_directories(doctest INTERFACE
    ${PROJECT_SOURCE_DIR}/tests/extern/doctest
)

add_executable(packet_test
    tests/reliable_udp/packet_test.cpp
)

target_link_libraries(packet_test PRIVATE
    knet
    doctest
)

target_compile_definitions(packet_test PRIVATE
    DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
)

set_target_properties(packet_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

add_test(NAME packet_test COMMAND packet_test)
