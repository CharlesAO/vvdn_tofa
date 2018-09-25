APPNAME = Cholesky


#MV_COMMON_BASE  ?= $(MV_TOOLS_DIR)/../mdk/common
MV_COMMON_BASE   ?= ../../../common


# project settings
MV_SOC_REV = ma2450

ifeq ($(MV_SOC_REV),ma2150)
  MVASMOPT +=-Dma2150
endif
ifeq ($(MV_SOC_REV),ma2450)
  MVASMOPT +=-Dma2450
endif

ComponentList := Cholesky
SHAVE_COMPONENTS = yes


# LEON rules
PROJECTCLEAN += \
  $(patsubst $(DirAppRoot)/%.c,$(DirAppObjBase)%.o,\
    $(wildcard $(DirAppRoot)/leon/*.c)) \
  $(patsubst $(DirAppRoot)/%.S,$(DirAppObjBase)%.o,\
    $(wildcard $(DirAppRoot)/leon/*.S))

 SHAVE_ASM_SOURCES_Cholesky_exit = $(wildcard $(DirAppRoot)/shave/*.asm)

SHAVE_GENASMS_asm_Cholesky_exit = \
  $(patsubst %.asm,$(DirAppObjBase)%_gen.asm,$(SHAVE_ASM_SOURCES_Cholesky_exit))

SHAVE_Cholesky_OBJS_exit = \
  $(patsubst $(DirAppObjBase)%_gen.asm,$(DirAppObjBase)%_shave.o, $(SHAVE_GENASMS_asm_Cholesky_exit))


CholeskyApp  =  shave/Cholesky

PROJECTCLEAN +=  $(SHAVE_GENASMS_asm_Cholesky_exit) $(SHAVE_Cholesky_OBJS_exit)
PROJECTINTERM += $(SHAVE_GENASMS_Cholesky)

SHAVE_APP_LIBS = $(CholeskyApp).mvlib
SHAVE0_APPS = $(CholeskyApp).shv0lib

PROJECTCLEAN += $(SHAVE_APP_LIBS) $(SHAVE0_APPS)

PROJECTCLEAN += output/swCommon.mvlib
PROJECTCLEAN += output/$(APPNAME).* output/$(APPNAME)_*.sym output/lst/$(APPNAME)_*.lst

# include the MDK build system
include $(MV_COMMON_BASE)/generic.mk

default: all
	@mv output/$(APPNAME).sym output/tmp
	@tail -n +5 output/tmp | head -n -2 \
	| grep -v '*ABS*' | grep -v '00000000 l    d  .debug_' \
	| grep -v 'S.shv0.cmx.text	00000000 __SVE' \
	| grep -v 'S.shv0.cmx.text	00000000 lrt___SVE' \
	| grep -v 'S.shv0.cmx.text	00000000 __CMX_SLICE' \
	| sort > output/$(APPNAME).sym
	@rm output/tmp

# set some convenient compiler options
CCOPT           +=
LDOPT           += -n
MVCC_OPT        += -O3 -ffast-math

# SHAVE linking rule
ENTRYPOINTS += -e gemvl_t -u potrf_ln -u syrk_tn_l -u trsvl_ln -u trsvl_lt -u --gc-sections

$(CholeskyApp).mvlib : $(SHAVE_Cholesky_OBJS) $(PROJECT_SHAVE_LIBS) $(SHAVE_Cholesky_OBJS_exit)
	$(ECHO) $(LD) $(MVLIBOPT) $(ENTRYPOINTS) $(SHAVE_Cholesky_OBJS) $(PROJECT_SHAVE_LIBS) $(SHAVE_Cholesky_OBJS_exit)  -o $@

