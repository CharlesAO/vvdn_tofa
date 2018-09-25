if(EXISTS "$ENV{MV_COMMON_BASE}")
    set(MV_COMMON_BASE $ENV{MV_COMMON_BASE})
else()
    set(MV_COMMON_BASE ${CMAKE_CURRENT_LIST_DIR}/../..)
endif(EXISTS "$ENV{MV_COMMON_BASE}")

set(XLINK_INCLUDE_DIRECTORIES
    ${MV_COMMON_BASE}/components/XLink/pc
    ${MV_COMMON_BASE}/components/XLink/shared
    ${MV_COMMON_BASE}/swCommon/include
    ${MV_COMMON_BASE}/shared/include
    /usr/include/libusb-1.0
    ${MV_COMMON_BASE}/swCommon/pcModel/half
)

set(XLINK_SOURCES
    ${MV_COMMON_BASE}/components/XLink/pc/XLinkPlatform.c
    ${MV_COMMON_BASE}/components/XLink/pc/usb_boot.c
    ${MV_COMMON_BASE}/components/XLink/shared/XLink.c
    ${MV_COMMON_BASE}/components/XLink/shared/XLinkDispatcher.c
    ${MV_COMMON_BASE}/swCommon/src/swcFifo.c
)

set(XLINK_PUBLIC_HEADERS
    ${MV_COMMON_BASE}/components/XLink/shared/XLink.h
    ${MV_COMMON_BASE}/components/XLink/shared/XLinkPublicDefines.h
)

