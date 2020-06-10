# CMakeList.txt : CMake project for RPiComponents, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)

project (rpisocket_test_project)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DESIGN_PATTERNS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppDesignPatterns CACHE INTERNAL "Path to include folder for CppDesignPatterns")
 
set(TEST_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/test.cpp
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories(${DESIGN_PATTERNS_INCLUDE_DIR})

add_subdirectory (external/CppDesignPatterns/design_patterns)

# Add source to this project's executable.
add_executable (rpisocket_test_project ${TEST_PROJECT_SRCS})

target_link_libraries(rpisocket_test_project RPiSocket)
find_library(pthread REQUIRED)
target_link_libraries(${PROJECT_NAME} pthread)
find_library(bluetooth REQUIRED)
target_link_libraries(${PROJECT_NAME} bluetooth)
find_library(design_patterns REQUIRED)
target_link_libraries(${PROJECT_NAME} design_patterns)