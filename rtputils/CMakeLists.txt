cmake_minimum_required(VERSION 3.5)

project(rtcrtputils)

set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

include_directories(
        ./
)

       
set(SRCS 
nack_handler.cpp
rtp_rtcp_handler.cpp
rtp_packetizer.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
