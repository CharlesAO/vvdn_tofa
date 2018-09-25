#commonbuilds.mk - generic.mk addition to be used with the Movidius elf build flow
#                  in order to add build rules for commonly used libraries
#
# Created on: Jul 17, 2013

#----------------------[ Software common library files ]---------------------------#
ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485 ma2490))
# MA2x8x doesn't support the Cdma Driver yet, this needs to move into an arch specific system soon
# for now we just specifically build swcShaveInit.c, ANSIWriteOverload.c and ignore swcCdmaShaveM2.c
# We cannot safely compile all code from socDrivers/shave and socDrivers/shared for ma2x8x.
 SH_SWCOMMON_C_SOURCES  = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/*/swcShaveInit.c)
 SH_SWCOMMON_C_SOURCES += $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/src/overloads/ANSIWriteOverload.c
else
 SH_SWCOMMON_C_SOURCES  = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/*/*.c)
 SH_SWCOMMON_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/src/*.c)
 SH_SWCOMMON_C_SOURCES += $(wildcard $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/src/*.c)
endif

SH_SWCOMMON_C_SOURCES += $(foreach var, $(SOC_REV_DIR), $(wildcard \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shave/arch/$(var)/src/*.c \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/sgl/src/*.c \
                           $(MV_DRIVERS_BASE)/$(MV_SOC_PLATFORM)/socDrivers/shared/arch/$(var)/hgl/src/*.c ))

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
# For now, MA2x8x support doesn't use the common SHAVE assembly as they do not use ma2x8x assembly mnemonics
SH_SWCOMMON_ASM_SOURCES = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/asm/ma2x8x/*.asm)
else ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2490))
# For now, MA2x8x support doesn't use the common SHAVE assembly as they do not use ma2x8x assembly mnemonics
SH_SWCOMMON_ASM_SOURCES = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/asm/ma2x9x/*.asm)
else
SH_SWCOMMON_ASM_SOURCES = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/asm/ma2x5x/*.asm)
endif

SH_SWCOMMON_CPP_SOURCES = $(wildcard $(MV_COMMON_BASE)/swCommon/shave_code/$(MV_SOC_PLATFORM)/*/*.cpp)

SH_SWCOMMON_GENASMS  = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SH_SWCOMMON_C_SOURCES))
SH_SWCOMMON_GENASMS += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SH_SWCOMMON_CPP_SOURCES))

SH_SWCOMMON_OBJS  = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SH_SWCOMMON_ASM_SOURCES))
SH_SWCOMMON_OBJS += $(patsubst %.asmgen,%_shave.o,$(SH_SWCOMMON_GENASMS))

SH_SWCOMMON_MBINOBJS  = $(patsubst %.asm,$(DirAppObjBase)%.mobj,$(SH_SWCOMMON_ASM_SOURCES))
SH_SWCOMMON_MBINOBJS += $(patsubst %.asmgen,%.mobj,$(SH_SWCOMMON_GENASMS))

#----------------------[ Components library files ]---------------------------#
SHAVE_COMPONENT_C_SOURCES = $(sort $(wildcard $(addsuffix /*.c,$(SH_COMPONENTS_PATHS))))
SHAVE_COMPONENT_CPP_SOURCES = $(sort $(wildcard $(addsuffix /*.cpp,$(SH_COMPONENTS_PATHS))))
SHAVE_COMPONENT_ASM_SOURCES = $(sort $(wildcard $(addsuffix /*.asm,$(SH_COMPONENTS_PATHS))))

SHAVE_COMPONENT_GENASMS  = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_COMPONENT_C_SOURCES))
SHAVE_COMPONENT_GENASMS += $(patsubst %.cpp,$(DirAppObjBase)%.asmgen,$(SHAVE_COMPONENT_CPP_SOURCES))

SHAVE_COMPONENT_OBJS  = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_COMPONENT_ASM_SOURCES))
SHAVE_COMPONENT_OBJS += $(patsubst %.asmgen,%_shave.o,$(SHAVE_COMPONENT_GENASMS))
#----------------------[ Actual build rules ]---------------------------#
# swCommon mlib
$(CommonMlibFile).mvlib: $(SH_SWCOMMON_OBJS)
	@echo "Shave Common LIB      : $@"
	@mkdir -p $(dir $@)
	$(ECHO) $(AR) rs $@ $(SH_SWCOMMON_OBJS)

MAXIMUM_NUMBER_OF_ARGUMENTS ?= 100

ifneq ("$(word $(MAXIMUM_NUMBER_OF_ARGUMENTS),$(SHAVE_COMPONENT_OBJS))","")

#define template for paritions of target
ProjectShaveLib_part = $(patsubst %.mvlib,%_,$(ProjectShaveLib))

define EOL


endef

define templateProjectShaveLib
# Collect generated partition lib names
ProjectShaveLibPartitions += $(ProjectShaveLib_part)$(1).a

.INTERMEDIATE: $$(ProjectShaveLib_part)$(1).a
$$(ProjectShaveLib_part)$(1).a : $(2)
	@echo "Creating temporary LIB : $$@"
	@mkdir -p $$(dir $$@)
	$$(ECHO)$$(AR) rs $$@ $(2)
endef

# A neat trick to split a command if it has more input parameters then $(MAXIMUM_NUMBER_OF_ARGUMENTS) files
$(eval _args:=)
$(eval index:=0)
$(foreach item,$(SHAVE_COMPONENT_OBJS),$(eval _args+=$(item))$(if $(word $(MAXIMUM_NUMBER_OF_ARGUMENTS),$(_args)),$(eval index=$(shell echo $$(($(index)+1))))$(eval $(call templateProjectShaveLib,$(index),$(_args)));$(EOL)$(eval _args:=)))
$(if $(_args),$(eval index=$(shell echo $$(($(index)+1))))$(eval $(call templateProjectShaveLib,$(index),$(_args))))

#The apps own lib
$(ProjectShaveLib) : $(ProjectShaveLibPartitions)
	@echo "Shave Component LIB   : $@"
	@mkdir -p $(dir $@)
	@echo "create $@" > $(dir $@)libaz.mri
	$(foreach partition,$(ProjectShaveLibPartitions),@echo "addlib $(partition)" >> $(dir $@)libaz.mri$(EOL))
	@echo "save" >> $(dir $@)libaz.mri
	@echo "end" >> $(dir $@)libaz.mri
	$(ECHO)$(AR) -M < $(dir $@)libaz.mri
	@rm -f $(dir $@)libaz.mri

else
#The apps own lib
$(ProjectShaveLib) : $(SHAVE_COMPONENT_OBJS)
	@echo "Shave Component LIB   : $@"
	@mkdir -p $(dir $@)
	$(ECHO)$(AR) rs $@ $(SHAVE_COMPONENT_OBJS)

endif


#---------------------[ Listing file builds ]----------------------------#
# These are not files built for executing however they are "common" and
# not related to the build flow. So instead of creating a new *.mk file
# or polluting the generic.mk file that contains build flow rules
# I'm adding these targets here

.INTERMEDIATE: $(DirAppOutput)/lst/$(APPNAME).elf.nodebug

# Generating a binutils generated version of the project
# This file contains valid leon disassembly, but doesn't decode SHAVE sections
ifeq ($(MV_CPU_ARCH),sparc)
LST_FILE=$(DirAppOutput)/lst/$(APPNAME)_leon.lst
else ifeq ($(MV_CPU_ARCH),armv8)
# ARMv8 objdump for lst file
LST_FILE=$(DirAppOutput)/lst/$(APPNAME)_arm.lst
endif

# Movidius Object dump of project elf file
# This file contains full disassembly of leon and shave sections
SHAVE_LST_FILE=$(DirAppOutput)/lst/$(APPNAME)_shave.lst

ifeq "$(JENKINS_HOME)" ""
$(LST_FILE) : $(DirAppOutput)/lst/$(APPNAME).elf.nodebug
	@echo "Dbg Listing      : $(DirAppOutput)/lst/$(APPNAME)_$(MV_CPU_ARCH).lst"
	$(ECHO) $(OBJDUMP) -xsrtd $< > $@

$(SHAVE_LST_FILE) : $(DirAppOutput)/lst/$(APPNAME).elf.nodebug
	@echo "Shave Dbg Listing     : $(DirAppOutput)/lst/$(APPNAME)_shave.lst"
#TODO: revert this workaround once the tools team removes bss section clashing checks
ifneq ($(strip $(shaveXApps)),)
	$(ECHO) $(MVDUMP) -cv:$(MV_SOC_REV) -shave -nosegmentcheck $< -o:$@  $(DUMP_NULL)
else
ifeq ($(MV_CPU_ARCH),sparc)
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2490))
	$(ECHO) $(MVDUMP) -cv:ma2480 $< -o:$@  $(DUMP_NULL)
    else
	$(ECHO) $(MVDUMP) -cv:$(MV_SOC_REV) $< -o:$@  $(DUMP_NULL)
    endif
endif
endif
else
$(LST_FILE) $(SHAVE_LST_FILE) :
	@echo $@ LST file generation is disabled on Jenkins environment
endif

lst: $(LST_FILE) $(SHAVE_LST_FILE)
	@echo > /dev/null

# This file contains all symbols from the elf file
$(DirAppOutput)/$(APPNAME).sym : $(DirAppOutput)/$(APPNAME).elf
	@echo "Symbols Listing  : $(DirAppOutput)/$(APPNAME).sym"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(OBJDUMP) -t $< > $@

ifneq ($(strip $(shaveXApps)),)

define cGroupVars_templateMK
#This target creates the dyanmical data loading infrastructure requires makefile include dependencies
appsGrpDynContextRules$(1).mk : Makefile
	@echo "" > $$@
	@echo "define cGroupAppVars_template$(1)" >> $$@
	@echo "" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'APPGROUPDYNDATASECTION=""$(1)grpdyndata'" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'APPGROUPDYNBSSDATASECTION=""__$(1)_groupbss_sectionStart'" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'APPGROUPDYNBSSDATASECTIONEND=""__$(1)_groupbss_sectionEnd'" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'GRPENTRY=""$(1)____AllTimeEntryPoint'" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'APPIATGROUP=""$(1)__iat_array_start'" >> $$@
	@echo "cDynContextDefs_""$$$$""(1) += -D'APPIATNAMESGROUP=""$(1)__iatnames_array_start'" >> $$@
	@echo "" >> $$@
	@echo "$$$$""(1)_GROUP_DATA := $$($(1)).shvZdata" >> $$@
	@echo "" >> $$@
	@echo "endef" >> $$@
	@echo "$$$$""(foreach appX,""$$$$""(MV_$(1)_APPS),""$$$$""(eval ""$$$$""(call cGroupAppVars_template""$(1)"",""$$$$""(call basenamenotdir,""$$$$""(appX)))))" >> $$@
	@echo "	" >> $$@
endef

$(foreach groupMK,$(MV_SHAVE_GROUPS),$(eval $(call cGroupVars_templateMK,$(groupMK))))

#generate a master C file holding the list of all applications in the project. Needed to get state of project.
$(DirAppObjBase)dynContextMaster.c : Makefile
	@mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$@))
	@echo "//  Generated file with all application content data" > $(call RESOLVE_TARGET_PATH,$@)
	@echo "#include \"theDynContext.h\"" >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "" >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "#define ___APPS_DETECTED " $(words $(shaveXApps))  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo ""  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo -e "$(foreach app,$(shaveXApps),extern DynamicContext_t $(call basenamenotdir,$(app))X_ModuleData;\n)" >> $(call RESOLVE_TARGET_PATH,$@)
	@echo ""  >>  $(call RESOLVE_TARGET_PATH,$@)
	@echo "DynamicContextInfo_t GlobalArray[___APPS_DETECTED]=" >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "{"  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo -e "   $(foreach app,$(shaveXApps), "{" &$(call basenamenotdir,$(app))X_ModuleData, "\""$(call basenamenotdir,$(app))"\"" "}," \n  )" >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "};"  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo ""  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "DynamicContextGlobal_t GlobalContextData="  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "{"   >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "     ___APPS_DETECTED, (DynamicContextInfo_t*)GlobalArray"  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "};"  >> $(call RESOLVE_TARGET_PATH,$@)
	@echo "" >> $(call RESOLVE_TARGET_PATH,$@)

endif

#Automatic rules for classic builds
define mvlibTemplate

$$(DirAppOutput)/$(1).mvlib : $$(SHAVE_$(1)_AUTOSTAT_OBJS) $$(PROJECT_SHAVE_LIBS)
	@echo "Building statically allocated shave lib $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	@if [ "$$(ECHO)" == "" ]; then echo "$$(LD) $$(MVLIBOPT) $$(DefaultStaticProjectShaveEntryPoints) --gc-sections $$(SHAVE_$(1)_AUTOSTAT_OBJS) $$(PROJECT_SHAVE_LIBS) $$(CompilerANSILibs) -o $$(call RESOLVE_TARGET_PATH,$$@) -Map $$(call RESOLVE_TARGET_PATH,$$@).map"; fi
	$$(ECHO) $$(LD) $$(MVLIBOPT) $$(DefaultStaticProjectShaveEntryPoints) --gc-sections $$(SHAVE_$(1)_AUTOSTAT_OBJS) $$(PROJECT_SHAVE_LIBS) $$(CompilerANSILibs) -o $$(call RESOLVE_TARGET_PATH,$$@) -Map $$(call RESOLVE_TARGET_PATH,$$@).map

endef

$(foreach app,$(AUTO_DETECTED_SHAVE_APPS),$(eval $(call mvlibTemplate,$(call basenamenotdir,$(app)))))

#Automatic rules for classic builds on shave NN
define mvlibNNTemplate

$$(DirAppOutput)/$(1).mvlib : $$(SHAVE_NN_$(1)_AUTOSTAT_OBJS)
	@echo "Building statically allocated shave NN lib $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	@if [ "$$(ECHO)" == "" ]; then echo "$$(LD) $$(MVLIBOPT) $$(DefaultStaticProjectShaveEntryPoints) --gc-sections $$(SHAVE_NN_$(1)_AUTOSTAT_OBJS) -o $$(call RESOLVE_TARGET_PATH,$$@) -Map $$(call RESOLVE_TARGET_PATH,$$@).map"; fi
	$$(ECHO) $$(LD) $$(MVLIBOPT) $$(DefaultStaticProjectShaveEntryPoints) --gc-sections $$(SHAVE_NN_$(1)_AUTOSTAT_OBJS) -o $$(call RESOLVE_TARGET_PATH,$$@) -Map $$(call RESOLVE_TARGET_PATH,$$@).map

endef

$(foreach app,$(AUTO_DETECTED_SHAVE_NN_APPS),$(eval $(call mvlibNNTemplate,$(call basenamenotdir,$(app)))))


#Adding rule to build mvlib dynamic files
define cAppDynamicMvlibBuildRule_template

$$(DirAppOutput)/$$($(1)App).mvlib :  $$(SHAVE_$(1)_AUTODYN_OBJS)
	@echo "Building dynamic data allocated shave lib $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	@if [ "$$(ECHO)" == "" ]; then echo "$$(LD) -e $$($(1)_MainEntry) --gc-sections  $$(MVLIBOPT) $$(SHAVE_$(1)_AUTODYN_OBJS)  $$(SHAVE_$(1)_DYN_APP_LIB) -o $$(call RESOLVE_TARGET_PATH,$$@)  -Map $$(call RESOLVE_TARGET_PATH,$$@).map"; fi
	$$(ECHO) $$(LD) -e $$($(1)_MainEntry) --gc-sections  $$(MVLIBOPT) $$(SHAVE_$(1)_AUTODYN_OBJS)  $$(SHAVE_$(1)_DYN_APP_LIB) -o $$(call RESOLVE_TARGET_PATH,$$@)  -Map $$(call RESOLVE_TARGET_PATH,$$@).map
endef

$(foreach app,$(shaveXApps),$(eval $(call cAppDynamicMvlibBuildRule_template,$(call basenamenotdir,$(app)),$(app))))

# Intermediate copy of the elf file with the key debug sections removed
# Manually selecting the debug sections to remove as I found the --strip-debug
# option too agressive
# This intermediate target is automatically deleted when make completes
$(DirAppOutput)/lst/$(APPNAME).elf.nodebug : $(DirAppOutput)/$(APPNAME).elf
	@mkdir -p $(DirAppOutput)/lst
	$(ECHO) $(OBJCOPY) -R .debug_aranges \
			   -R .debug_ranges \
			   -R .debug_pubnames \
			   -R .debug_info \
			   -R .debug_abbrev \
			   -R .debug_asmline \
			   -R .debug_line \
			   -R .debug_str \
			   -R .debug_loc \
			   -R .debug_macinfo \
			$< $@


###################################################################
# Dependencies
###################################################################

ifeq ($(MAKEDEPEND),yes)

ifeq ($(MV_CPU_ARCH),sparc)
-include $(LEON_APP_DEPS)
else ifeq ($(MV_CPU_ARCH),armv8)
-include $(ARMV8_APP_DEPS)
endif
-include $(LEON_RT_DEPS)
-include $(SHAVE_APP_DEPS)

endif
