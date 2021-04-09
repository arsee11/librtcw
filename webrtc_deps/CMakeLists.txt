cmake_minimum_required(VERSION 3.5)

project(webrtc_deps)


add_subdirectory(webrtc)

set(SRCS
pc/jsep_session_description.cc
pc/webrtc_sdp.cc
pc/media_session.cc
)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME}
    -Wl,--whole-archive
    webrtc_base
    webrtc_p2p
    webrtc_modules
    -Wl,--no-whole-archive
)
