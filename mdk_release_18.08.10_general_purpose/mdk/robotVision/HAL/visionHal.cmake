cmake_minimum_required(VERSION 2.8)
find_package( OpenCV REQUIRED )
add_definitions(-D__PC__)
add_definitions(-DUSE_USB_VSC)
set (CMAKE_CXX_STANDARD 11)


if(EXISTS "$ENV{MV_COMMON_BASE}")
    set(MV_COMMON_BASE $ENV{MV_COMMON_BASE})
else()
    set(MV_COMMON_BASE ${CMAKE_CURRENT_LIST_DIR}/../../common)
endif(EXISTS "$ENV{MV_COMMON_BASE}")


if(${VTRACK} MATCHES "1")
    message("Using VTRACK sources")
    include(${MV_COMMON_BASE}/components/vTrack/vTrack.cmake)
endif()

if(${FIRF} MATCHES "1")
    message("Using FIRF sources")
    include(${MV_COMMON_BASE}/components/FIRF/Firf.cmake)
endif()
if(${STEREO} MATCHES "1")
    message("Using Stereo")
endif()


include(${MV_COMMON_BASE}/components/XLink/XLink.cmake)

    #set Directories with moviEval Components
    set(moviEvalSourcesDir ${CMAKE_CURRENT_LIST_DIR}/source/common)
    set(moviEvalSourcesDirVtrack ${CMAKE_CURRENT_LIST_DIR}/source/VTRACK)
    set(moviEvalSourcesDirFirf ${CMAKE_CURRENT_LIST_DIR}/source/FIRF)
    set(moviEvalSourcesDirStereo ${CMAKE_CURRENT_LIST_DIR}/source/STEREO)
    set(pcModelDir ${MV_COMMON_BASE/swCommon/pcModel/half})

    #find source file
    FILE(GLOB moviEvalSources ${moviEvalSourcesDir}/*.cpp ${moviEvalSourcesDir}/*.c)
    FILE(GLOB mainSourcesVtrack ${moviEvalSourcesDirVtrack}/*.cpp)
    FILE(GLOB mainSourcesStereo ${moviEvalSourcesDirStereo}/*.cpp)
    FILE(GLOB mainSourcesFirf ${moviEvalSourcesDirFirf}/*.cpp)

set(MOVIEVAL_INCLUDE_DIRECTORIES
    ${MV_COMMON_BASE}/components/flic/plugins/plgXlinkConfig/shared
    ${MV_COMMON_BASE}/components/visionBaseFlicPlg/leon
    ${MV_COMMON_BASE}/components/vTrack/source/apiLite/
    ${MV_COMMON_BASE}/components/DataCollector/LOS
    ${MV_COMMON_BASE}/components/DataCollector/shared
    ${MV_COMMON_BASE}/drivers/myriad2/icDrivers/osDrivers/include
    ${moviEvalSourcesDir}
    ${XLINK_INCLUDE_DIRECTORIES}
    ${${ALGORITHM}_INCLUDE_DIRECTORIES}
)
set(MOVIEVAL_SOURCES ${moviEvalSources} ${XLINK_SOURCES})
set(MOVIEVAL_LIBS "${MV_COMMON_BASE}/../packages/movidius/pcModel/moviCompile/compilerIntrinsics/lib/linux64/libintrinsics.a"
    "${MV_COMMON_BASE}/../packages/movidius/pcModel/moviCompile/compilerVectorFunctions/lib/linux64/libvectorfunctions.a"
    ${OpenCV_LIBS} ${LIBUSB_1_LIBRARIES} -lusb-1.0 -lpthread)

if(${FIRF} MATCHES "1")
    list(APPEND MOVIEVAL_INCLUDE_DIRECTORIES ${moviEvalSourcesDirFirf} ${FIRF_INCLUDE_DIRECTORIES})
    list(APPEND MOVIEVAL_SOURCES ${mainSourcesVtrack} ${FIRF_SOURCES} ${FIRF_DRAWING_SOURCES})

endif()
if(${STEREO} MATCHES "1")
    set(STEREO_INCLUDE_DIRECTORIES ${pcModelDir})
    FILE(GLOB STEREO_DRAWING_SOURCES ${pcModelDir}/*.cpp)
    list(APPEND MOVIEVAL_INCLUDE_DIRECTORIES ${moviEvalSourcesDirStereo} ${STEREO_INCLUDE_DIRECTORIES})
    list(APPEND MOVIEVAL_SOURCES ${mainSourcesStereo} ${STEREO_SOURCES} ${STEREO_DRAWING_SOURCES})

endif()
if(${VTRACK} MATCHES "1")
    list(APPEND MOVIEVAL_INCLUDE_DIRECTORIES ${moviEvalSourcesDirVtrack} ${VTRACK_INCLUDE_DIRECTORIES})
    list(APPEND MOVIEVAL_SOURCES ${mainSourcesVtrack} ${VTRACK_SOURCES} ${VTRACK_DRAWING_SOURCES})
    list(APPEND MOVIEVAL_LIBS ${VTRACK_LIBS})

endif()


set(CMAKE_CXX_FLAGS "-fpermissive")
