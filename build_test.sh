#!/usr/bin/env bash

g++ -std=c++23 \
    -I.\
    tests/reliable_udp/packet_test.cpp -o tests/build/reliable_udp/packet_test

tests/build/reliable_udp/packet_test
