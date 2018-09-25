set(APP_NAME flicpcapp)
set(MV_COMMON_BASE ../../../../../common)
cmake_minimum_required(VERSION 2.8)

project(${APP_NAME})

add_definitions(-D__PC__)
add_definitions(-DUSE_USB_VSC)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package( OpenCV REQUIRED )


include(${MV_COMMON_BASE}/components/XLink/XLink.cmake)
include(${MV_COMMON_BASE}/components/vTrack/vTrack.cmake)


include_directories(
    ${XLINK_INCLUDE_DIRECTORIES}
    ${VTRACK_INCLUDE_DIRECTORIES}
    ${vTrackResultsApi}
    ./backends/xlink/
    ./frontends/cvdisplay/
    ./frontends/hevcdecode/
    ./modules/common/
    ./modules/imgShow/
    ./modules/Stereo/
    ./modules/vTrack/
    ./apps/demo1/
    ./apps/simpleDisplayNv12/
    ./apps/bitstreamtofile/
    /usr/include/ffmpeg/
)

add_executable( ${APP_NAME}
    main.cpp
    ./backends/xlink/be_xlink.cpp
    ./backends/xlink/RecMq.cpp
    ./backends/xlink/RecFifo.cpp
    ./backends/xlink/Rec2File.cpp
    ./frontends/cvdisplay/fe_cvdisplay.cpp
    ./modules/imgShow/imgShow.cpp
    ./modules/common/helper.cpp
    ./modules/Stereo/stereoPostProcNode.cpp
    ./modules/vTrack/vTrackNode.cpp
    ./modules/vTrack/visualizer.cpp
    ./modules/vTrack/VtrackDrawer.cpp
    ./modules/vTrack/VtrackDrawerMest.cpp
    ./modules/vTrack/VtrackDrawerMoviOf.cpp
    ./frontends/hevcdecode/fe_hevc.cpp
    ${XLINK_SOURCES}
    ${VTRACK_DRAWING_SOURCES}
    ./apps/demo1/demo1.cpp
    ./apps/simpleDisplayNv12/simpleDisplayNv12.cpp
    ./apps/bitstreamtofile/bitstrtofile.cpp
)

target_link_libraries(
    ${APP_NAME}
    ${OpenCV_LIBS}
    ${LIBAV_LIBRARIES}
    -lusb-1.0
    -lpthread
    -lm
    -lavcodec
    -lavformat
    -lavutil
    -lswscale
    -lrt
)
