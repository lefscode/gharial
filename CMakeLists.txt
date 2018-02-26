cmake_minimum_required (VERSION 3.0.2)
project (gharial)
include(CheckCXXCompilerFlag)

option(GHARIAL_BUILD_TESTS "build tests alongside the project" OFF)

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

file (GLOB GHARIAL_MAIN "src/main.cpp")
file (GLOB GHARIAL_SRC "src/delegate.cpp")
file (GLOB HASHDIS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/hashidsxx/hashids.cpp)

file (GLOB PISTACHE_COMMON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/src/common/*.cc")
file (GLOB PISTACHE_SERVER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pistache/src/server/*.cc")

set(SOURCE_FILES
    ${GHARIAL_SRC}
    ${HASHDIS_SRC}
    ${PISTACHE_COMMON_SOURCE_FILES}
    ${PISTACHE_SERVER_SOURCE_FILES}
)

add_executable(${PROJECT_NAME} ${GHARIAL_MAIN} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} pthread)

function(gharial_test test_name)

    set(TEST_EXECUTABLE run_${test_name})
    set(TEST_SOURCE tests/${test_name}.cpp)

    add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE} ${HASHDIS_SRC}  ${PISTACHE_COMMON_SOURCE_FILES} ${PISTACHE_SERVER_SOURCE_FILES})
    target_link_libraries(${TEST_EXECUTABLE} pthread)
    add_test(${test_name} ${TEST_EXECUTABLE})

endfunction()


if (GHARIAL_BUILD_TESTS)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Catch2/single_include)
    enable_testing()
    
    #testcases
    gharial_test(test-01)

endif()

