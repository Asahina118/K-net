add_library(doctest INTERFACE)

target_include_directories(doctest INTERFACE
    ${PROJECT_SOURCE_DIR}/tests/extern/doctest
)

add_executable(knet_test
    ${PROJECT_SOURCE_DIR}/tests/main.cpp

    ${PROJECT_SOURCE_DIR}/tests/reliable_udp/packet_test.cpp
    ${PROJECT_SOURCE_DIR}/tests/reliable_udp/socket_test.cpp
)

target_link_libraries(knet_test PRIVATE
    knet
    doctest
)

# target_compile_definitions(knet_test PRIVATE
#     DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
# )
#

set_target_properties(knet_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

add_test(NAME knet_test COMMAND knet_test)
