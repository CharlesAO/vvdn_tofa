cmake_minimum_required(VERSION 2.8)

project( dataCollection )

add_definitions(-DGUI_VISUALIZER)

set(VTRACK "1")
set(STEREO "1")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../HAL/visionHal.cmake)

include_directories(${MOVIEVAL_INCLUDE_DIRECTORIES})
add_executable( vTrackStereo
    ${CMAKE_CURRENT_SOURCE_DIR}/../../PcViz/vTrack_Stereo/main.cpp
    ${MOVIEVAL_SOURCES}
)

target_link_libraries( vTrackStereo
    ${MOVIEVAL_LIBS}
)

set(CMAKE_CXX_FLAGS "-fpermissive")
