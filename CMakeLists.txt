cmake_minimum_required(VERSION 3.5)

project(rtcw LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS " --std=c++20 -fPIC -g ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS " -fPIC -g ${CMAKE_C_FLAGS}")


add_definitions(-D WEBRTC_POSIX)
add_definitions(-D WEBRTC_USE_EPOLL)
add_definitions(-D WEBRTC_LINUX)

include_directories(webrtc_deps/)
include_directories(webrtc_deps/absl_inl/absl)
include_directories(webrtc_deps/webrtc)
include_directories(webrtc_deps/third_party/libyuv/include)
include_directories(webrtc_deps/third_party/libsrtp/include)
include_directories(webrtc_deps/third_party/libsrtp/crypto/include)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(../)

link_directories(webrtc_deps/third_party/libyuv)
link_directories(webrtc_deps/third_party/libsrtp)


add_subdirectory(webrtc_deps/)

add_library(${PROJECT_NAME} SHARED
    librtcw_global.h
    #timer.cpp
    #time_clocks.cpp
    #source.cpp
    media_def.cpp
    media_frame.cpp
    sdp_info.cpp
    rtp_demuxer.cpp
    rtp_muxer.cpp
)

target_link_libraries(${PROJECT_NAME}
    -Wl,--whole-archive
    webrtc_deps
    webrtc_api
    webrtc_base
    webrtc_common_media
    webrtc_logging
    webrtc_modules
    webrtc_p2p
    webrtc_pc
    webrtc_system_wrappers
    -Wl,--no-whole-archive
)

target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRTCW_LIBRARY)
