#!/bin/bash

mkdir build

export BUILD_TEST=ON

cmake -S rpisocket/ -B build/

cmake --build ./build || exit 1

export SOCKET_PORT=8889


./rpisocket/build/rpisocket_test_project