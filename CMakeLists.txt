cmake_minimum_required(VERSION 3.5)

project(librtcw LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS " --std=c++20 -fPIC -g ${CMAKE_CXX_FLAGS}")


add_definitions(-D WEBRTC_POSIX)
add_definitions(-D WEBRTC_USE_EPOLL)
add_definitions(-D WEBRTC_LINUX)

include_directories(webrtc_deps/)
include_directories(webrtc_deps/absl_inl)
include_directories(webrtc_deps/webrtc)
include_directories(third_party/libsrtp/include/)
include_directories(third_party/libsrtp/crypto/include)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(../)


add_subdirectory(webrtc_deps/)
#add_subdirectory(rtputils/)
#add_subdirectory(signaling/)
#add_subdirectory(session/)
#add_subdirectory(transport/)


add_library(rtcw SHARED
    librtcw_global.h
    #timer.cpp
    #time_clocks.cpp
    #source.cpp
    media_def.cpp
    media_frame.cpp
    sdp_info.cpp
    rtp_demuxer.cpp
)

target_link_libraries(rtcw
    -Wl,--whole-archive
    #rtcsignaling
    #rtcsession
    #rtctransport
    #rtcrtputils
    -Wl,--no-whole-archive
)

target_compile_definitions(rtcw PRIVATE LIBRTCW_LIBRARY)
