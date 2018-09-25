#
# BSP specific settings. To be included in application Makefiles
#

RTEMS_BSP = ma2100

prefix = /home/passass/work/mdk/common/utils/RTEMSbuild/StagingArea/output
exec_prefix = /home/passass/work/mdk/common/utils/RTEMSbuild/StagingArea/output/myriad2-sparc-rtems

CC_FOR_TARGET = myriad2-sparc-rtems-gcc --pipe
CXX_FOR_TARGET = myriad2-sparc-rtems-g++
AS_FOR_TARGET = myriad2-sparc-rtems-as
AR_FOR_TARGET = myriad2-sparc-rtems-ar
NM_FOR_TARGET = myriad2-sparc-rtems-nm
LD_FOR_TARGET = myriad2-sparc-rtems-ld
SIZE_FOR_TARGET = myriad2-sparc-rtems-size
OBJCOPY_FOR_TARGET = myriad2-sparc-rtems-objcopy

RTEMS_API = @RTEMS_API@

CC= $(CC_FOR_TARGET)
CXX= $(CXX_FOR_TARGET)
AS= $(AS_FOR_TARGET)
LD= $(LD_FOR_TARGET)
NM= $(NM_FOR_TARGET)
AR= $(AR_FOR_TARGET)
SIZE= $(SIZE_FOR_TARGET)
OBJCOPY= $(OBJCOPY_FOR_TARGET)

export CC
export CXX
export AS
export LD
export NM
export AR
export SIZE
export OBJCOPY

RTEMS_ROOT = $(prefix)
PROJECT_ROOT = $(RTEMS_ROOT)
RTEMS_CUSTOM = $(RTEMS_ROOT)/make/custom/$(RTEMS_BSP).cfg
RTEMS_SHARE = $(RTEMS_ROOT)/share/rtems$(RTEMS_API)

RTEMS_USE_OWN_PDIR = no
RTEMS_HAS_POSIX_API = yes
RTEMS_HAS_ITRON_API = no
RTEMS_HAS_CPLUSPLUS = yes

export RTEMS_BSP
export RTEMS_CUSTOM
export PROJECT_ROOT

