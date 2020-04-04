# CMakeList.txt : CMake project for RPiSocket
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)
set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (RPiSocket VERSION 0.0.42)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
 
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(RPISOCKET_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/btserver.cpp
${PROJECT_SOURCE_DIR}/server.cpp
${PROJECT_SOURCE_DIR}/wifiserver.cpp
)
 
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${RPISOCKET_PROJECT_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}
)
