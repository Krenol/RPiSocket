#!/bin/bash
set -e

#set vars
BUILD_DIR=/data/build
EXEC=/data/rpisocket/build/rpisocket_test_project

# SET BUILD_TEST IF IT WASN'T SET
if [ "$BUILD_TEST" != 'ON' ] && [ "$BUILD_TEST" != 'OFF' ]; then
    BUILD_TEST=ON
fi

echo "BUILD_TEST=$BUILD_TEST"
echo "SOCKET_PORT=$SOCKET_PORT"

# RUN INPUT
if [ "$1" = 'start' ] || [ "$1" = 'build' ]; then
    cmake -D BUILD_TEST=$BUILD_TEST --build $BUILD_DIR
    cmake --build $BUILD_DIR
    if [ "$1" = 'start' ]; then
        $EXEC
    fi
else 
    exec "$@"
fi