cmake_minimum_required(VERSION 2.8)
project( XLink )

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-D__PC__)
add_definitions(-DUSE_USB_VSC)
set(CMAKE_CXX_STANDARD 11)

include(XLink.cmake)

if(EXISTS "$ENV{MV_COMMON_BASE}")
    set(MV_COMMON_BASE $ENV{MV_COMMON_BASE})
else()
    set(MV_COMMON_BASE ${CMAKE_CURRENT_SOURCE_DIR}/../../)
endif(EXISTS "$ENV{MV_COMMON_BASE}")

set(PUBLIC_HEADERS
        ${XLINK_PUBLIC_HEADERS}
)

include_directories(
        ${XLINK_INCLUDE_DIRECTORIES}
)

add_library( XLink SHARED
        ${XLINK_SOURCES}
)
target_link_libraries(XLink pthread usb-1.0)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${PUBLIC_HEADERS} DESTINATION include)
