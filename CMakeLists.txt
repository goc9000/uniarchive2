cmake_minimum_required(VERSION 3.3)
project(uniarchive2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCE_FILES src/main.cpp)
add_executable(uniarchive2 ${SOURCE_FILES})

target_link_libraries(uniarchive2 Qt5::Core)

target_compile_definitions(uniarchive2 PRIVATE TEST_DATA_DIR=${TEST_DATA_DIR})
