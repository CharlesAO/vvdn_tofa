MV_COMMON_BASE?=../../../../../common
C_SOURCES = main.c ../shared/commonFuncs.c \
$(MV_COMMON_BASE)/components/XLink/pc/XLinkPlatform.c \
$(MV_COMMON_BASE)/components/XLink/pc/usb_boot.c \
$(MV_COMMON_BASE)/components/XLink/shared/XLink.c \
$(MV_COMMON_BASE)/components/XLink/shared/XLinkDispatcher.c

MVCMD_NAME?=$(PWD)/../myriad/output/myriad.mvcmd
ECHO=@

CCOPT += -D'USE_USB_VSC' 

$(info Using mvcmd at:        $(MVCMD_NAME))

all: ${C_SOURCES} Makefile
	$(ECHO)gcc $(CCOPT)  -Wall ${C_SOURCES} -DMVCMD_NAME="\"$(MVCMD_NAME)\"" \
			-I ../shared/ \
			-I $(MV_COMMON_BASE)/components/XLink/pc/ \
			-I $(MV_COMMON_BASE)/components/XLink/shared/ \
			-I $(MV_COMMON_BASE)/shared/include/ \
			-I /usr/include/libusb-1.0  \
			-D __PC__ -o XLink \
			-lusb-1.0 -lstdc++ -lpthread -ggdb