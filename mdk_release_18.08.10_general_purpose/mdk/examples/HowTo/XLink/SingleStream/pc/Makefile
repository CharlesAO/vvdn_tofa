.SUFFIXES:

MV_COMMON_BASE?=../../../../../common
C_SOURCES = main.c \
	$(MV_COMMON_BASE)/components/XLink/shared/XLink.c \
	$(MV_COMMON_BASE)/components/XLink/shared/XLinkDispatcher.c \
	$(MV_COMMON_BASE)/components/XLink/pc/usb_boot.c \
	$(MV_COMMON_BASE)/components/XLink/pc/XLinkPlatform.c

C_OBJS=$(patsubst %.c,%.o,$(C_SOURCES))

MVCMD_NAME?=$(PWD)/../myriad/output/myriad.mvcmd
ECHO=@
CCOPT += \
			-DMVCMD_NAME="\"$(MVCMD_NAME)\"" \
			-I $(MV_COMMON_BASE)/components/XLink/pc/ \
			-I $(MV_COMMON_BASE)/components/XLink/shared/ \
			-I $(MV_COMMON_BASE)/shared/include/ \
			-I /usr/include/libusb-1.0  \
			-I $(MV_COMMON_BASE)/swCommon/pcModel/half \
			-D __PC__ \
			-ggdb

#Used just for this application
CCOPT += -D'USE_USB_VSC' 

$(info Using mvcmd at:        $(MVCMD_NAME))

%.o: %.c Makefile
	$(ECHO)$(CC) $(CCOPT)  -Wall $< -c -o $@

XLink: $(C_OBJS)
	$(ECHO)$(CXX) $(CCOPT) $^ -o $@ \
			-lusb-1.0 -lstdc++ -lpthread

all: XLink
	@echo "Done building."

