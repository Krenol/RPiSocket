#!/bin/bash
set -e

#set vars
EXEC=/data/build/lib/rpisocket_test_project

# RUN INPUT
if [ "$1" = 'start' ]; then
    $EXEC
else 
    exec "$@"
fi