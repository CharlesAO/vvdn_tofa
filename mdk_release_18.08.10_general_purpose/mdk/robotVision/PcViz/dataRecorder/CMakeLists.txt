cmake_minimum_required(VERSION 2.8)
project( dataCollection )


include(${CMAKE_CURRENT_SOURCE_DIR}/../../HAL/visionHal.cmake)

include_directories(${MOVIEVAL_INCLUDE_DIRECTORIES})
add_executable( dataRecorder
    main.cpp
    ${MOVIEVAL_SOURCES}
)

target_link_libraries( dataRecorder
    ${MOVIEVAL_LIBS}
)

set(CMAKE_CXX_FLAGS "-fpermissive")
