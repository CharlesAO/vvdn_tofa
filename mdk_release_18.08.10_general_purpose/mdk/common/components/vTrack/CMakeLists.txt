cmake_minimum_required(VERSION 2.8)
project(vTrackAndDeps )
find_package( OpenCV REQUIRED )
add_definitions(-D__PC__)

include(vTrack.cmake)

include_directories(
    ${VTRACK_INCLUDE_DIRECTORIES}
)

add_library(${PROJECT_NAME} SHARED
    ${VTRACK_SOURCES}
)

target_link_libraries(${PROJECT_NAME} ${VTRACK_LIBS} ${OpenCV_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${VTRACK_PUBLIC_HEADERS} DESTINATION include)

if(NOT WITHOUT_DRAWING)
    set(PUBLIC_HEADERS_drawing
        ${VTRACK_DRAWING_PUBLIC_HEADERS}
    )
    add_library(vTrack_drawing SHARED
        ${VTRACK_DRAWING_SOURCES}
    )

    target_link_libraries(vTrack_drawing ${PROJECT_NAME} ${OpenCV_LIBS})

    install(TARGETS vTrack_drawing DESTINATION lib)
    install(FILES PUBLIC_HEADERS_drawing DESTINATION include)
endif()
