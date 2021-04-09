cmake_minimum_required(VERSION 3.5)

project(rtctransport)

set(CMAKE_CXX_FLAGS " -fPIC ${CMAKE_CXX_FLAGS}")

include_directories(
        ./
)

       
set(sSRCS
transport.cpp
)

add_library(${PROJECT_NAME} STATIC ${sSRCS})
