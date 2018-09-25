
# set MV_COMMON_BASE
MV_COMMON_BASE ?= ../../../../../common

# set platform
MV_SOC_PLATFORM = myriad2

# Extra app related options
ifeq ($(MV_SOC_PLATFORM), myriad2)
ComponentList_LOS +=UnitTestVcs VcsHooks PipePrint
endif

# choosing if this project has shave components or not
SHAVE_COMPONENTS = no

# choosing the C sources of the dummy application on shave
DummyApp = shave/dummy
SHAVE_C_SOURCES_dummy = $(wildcard ./shave/*.c)

# required generated assembly files for the dummy app on shave
SHAVE_GENASMS_dummy = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_dummy))

# generating required objects from souces
SHAVE_dummy_OBJ = $(patsubst %.asmgen,%_shave.o,$(SHAVE_GENASMS_dummy))
SHAVE_dummy_OBJ += $(patsubst %.asm,$(DirAppObjBase)%_shave.o, $(wildcard ../../*.asm))

# update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_dummy) $(SHAVE_dummy_OBJ)

# uncomment below to reject generated shave as intermediary files
PROJECTINTERM += $(SHAVE_GENASMS_dummy)

SHAVE_APP_LIBS = $(DummyApp).mvlib
SHAVE0_APPS = $(DummyApp).shv0lib
MVCC_INCLUDE += -I ../../

# include the generic Makefile
include $(MV_COMMON_BASE)/generic.mk

$(DummyApp).mvlib : $(SHAVE_dummy_OBJ) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(SHAVE_dummy_OBJ) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@



TEST_TAGS := "MA2150, MA2450"
