cmake_minimum_required(VERSION 3.5)

project(rtcsession)

set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

include_directories(
        ./
)

       
set(SRCS 
stream.cpp
stream_events.cpp
stream_factory.cpp
video_stream.cpp
peer_session.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
