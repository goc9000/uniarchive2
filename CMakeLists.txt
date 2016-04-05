cmake_minimum_required(VERSION 3.3)
project(uniarchive2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5 REQUIRED COMPONENTS Core)

set(SOURCE_FILES
        src/extraction/yahoo/extract_yahoo_messenger_conversations.cpp
        src/extraction/yahoo/extract_yahoo_messenger_conversations.h
        src/extraction/yahoo/ExtractYahooProtocolEventsIterator.cpp
        src/extraction/yahoo/ExtractYahooProtocolEventsIterator.h
        src/extraction/yahoo/YahooProtocolEvent.cpp
        src/extraction/yahoo/YahooProtocolEvent.h
        src/intermediate_format/content/IntermediateFormatMessageContent.cpp
        src/intermediate_format/content/IntermediateFormatMessageContent.h
        src/intermediate_format/events/IFDeclineConferenceEvent.cpp
        src/intermediate_format/events/IFDeclineConferenceEvent.h
        src/intermediate_format/events/IFJoinConferenceEvent.cpp
        src/intermediate_format/events/IFJoinConferenceEvent.h
        src/intermediate_format/events/IFLeaveConferenceEvent.cpp
        src/intermediate_format/events/IFLeaveConferenceEvent.h
        src/intermediate_format/events/IFStartConversationEvent.cpp
        src/intermediate_format/events/IFStartConversationEvent.h
        src/intermediate_format/events/IntermediateFormatEvent.cpp
        src/intermediate_format/events/IntermediateFormatEvent.h
        src/intermediate_format/subjects/ApparentSubject.cpp
        src/intermediate_format/subjects/ApparentSubject.h
        src/intermediate_format/subjects/SubjectGivenAsAccount.cpp
        src/intermediate_format/subjects/SubjectGivenAsAccount.h
        src/intermediate_format/ApparentTime.cpp
        src/intermediate_format/ApparentTime.h
        src/intermediate_format/IntermediateFormatConversation.cpp
        src/intermediate_format/IntermediateFormatConversation.h
        src/protocols/yahoo/utils.cpp
        src/protocols/yahoo/utils.h
        src/protocols/ArchiveFormats.cpp
        src/protocols/ArchiveFormats.h
        src/protocols/FullAccountName.cpp
        src/protocols/FullAccountName.h
        src/protocols/IMProtocols.cpp
        src/protocols/IMProtocols.h
        src/utils/external_libs/optional.hpp
        src/utils/language/invariant.h
        src/utils/language/JavaStyleIterator.h
        src/main.cpp
        )

add_executable(uniarchive2 ${SOURCE_FILES})

target_link_libraries(uniarchive2 Qt5::Core)
target_include_directories(uniarchive2 PRIVATE src)
target_compile_definitions(uniarchive2 PRIVATE TEST_DATA_DIR=${TEST_DATA_DIR})
