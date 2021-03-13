project (rpisocket VERSION 0.0.42)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(RPISOCKET_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/btserver.cpp
${PROJECT_SOURCE_DIR}/server.cpp
${PROJECT_SOURCE_DIR}/wifiserver.cpp
)

add_library(${PROJECT_NAME} SHARED ${RPISOCKET_PROJECT_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}
)

if (NOT TARGET design_patterns)
    add_subdirectory (external/CppDesignPatterns)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE design_patterns)


if(${BUILD_TEST})
    add_subdirectory(test)
endif()
