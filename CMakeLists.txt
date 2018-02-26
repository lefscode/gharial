cmake_minimum_required (VERSION 3.0.2)
project (gharial)
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories (
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json/include 
${CMAKE_CURRENT_SOURCE_DIR}/src
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/hashidsxx
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/include
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Clara/include
)
#include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

file (GLOB GHARIAL_SRC "src/*.cpp")
file (GLOB HASHDIS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/hashidsxx/hashids.cpp)

file (GLOB PISTACHE_COMMON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/src/common/*.cc")
file (GLOB PISTACHE_SERVER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/src/server/*.cc")
#file (GLOB PISTACHE_CLIENT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/client/*.cc")

set(SOURCE_FILES
    ${GHARIAL_SRC}
    ${HASHDIS_SRC}
    ${PISTACHE_COMMON_SOURCE_FILES}
    ${PISTACHE_SERVER_SOURCE_FILES}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} pistache pthread)
target_link_libraries(${PROJECT_NAME} pthread)
