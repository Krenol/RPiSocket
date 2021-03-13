#!/bin/bash

mkdir build

export BUILD_TEST=ON

cmake -S . -B build/

cmake --build ./build || exit 1

export SOCKET_PORT=8889


./build/lib/rpisocket_test_project