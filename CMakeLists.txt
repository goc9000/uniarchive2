cmake_minimum_required(VERSION 3.3)
project(uniarchive2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCE_FILES
        src/extraction/yahoo/ExtractYahooProtocolEventsIterator.cpp
        src/extraction/yahoo/ExtractYahooProtocolEventsIterator.h
        src/extraction/yahoo/YahooProtocolEvent.cpp
        src/extraction/yahoo/YahooProtocolEvent.h
        src/protocols/yahoo/utils.cpp
        src/protocols/yahoo/utils.h
        src/protocols/ArchiveFormats.h
        src/protocols/ArchiveFormats.cpp
        src/protocols/IMProtocols.h
        src/protocols/IMProtocols.cpp
        src/utils/external_libs/optional.hpp
        src/utils/language/invariant.h
        src/utils/language/JavaStyleIterator.h
        src/main.cpp
        )

add_executable(uniarchive2 ${SOURCE_FILES})

target_link_libraries(uniarchive2 Qt5::Core)
target_include_directories(uniarchive2 PRIVATE src)
target_compile_definitions(uniarchive2 PRIVATE TEST_DATA_DIR=${TEST_DATA_DIR})
