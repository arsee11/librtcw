cmake_minimum_required(VERSION 3.5)

project(sdp_gen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS " --std=c++20 -fPIC -g ${CMAKE_CXX_FLAGS}")


add_definitions(-D WEBRTC_POSIX)
add_definitions(-D WEBRTC_USE_EPOLL)
add_definitions(-D WEBRTC_LINUX)

include_directories(../../webrtc_deps/)
include_directories(../../webrtc_deps/webrtc)
include_directories(../../)

link_directories(../../lib)

add_executable(${PROJECT_NAME} 
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    rtcw
    ssl
    crypto
    yuv
)
