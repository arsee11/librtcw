cmake_minimum_required(VERSION 3.5)

project(rtcsignaling)

set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

include_directories(
        ./
)

       
set(SRCS 
signaling.cpp
signaling_msg.cpp
json_signaling_client.cpp
#http_signaling_client.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
