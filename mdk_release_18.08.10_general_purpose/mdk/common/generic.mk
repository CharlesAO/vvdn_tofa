#generic.mk - simple generic.mk to be used with the Movidius one stage build flow
#
# Created on: Jul 17, 2013

# Need some tentative definitions first and we need to detect the tools to be used

###################################################################
#       Using settings makefiles for wider system settings        #
###################################################################
include $(MV_COMMON_BASE)/generalsettings.mk
include $(MV_COMMON_BASE)/toolssettings.mk
include $(MV_COMMON_BASE)/includessettings.mk
include $(MV_COMMON_BASE)/commonsources.mk
include $(MV_COMMON_BASE)/saferemove.mk
###################################################################
#       And finally listing all of the build rules                #
###################################################################

# detect and correctly handle clean target when parrallel building
ifneq ($(filter clean,$(MAKECMDGOALS)),)
OTHERGOALS := $(filter-out clean,$(MAKECMDGOALS))

ifneq ($(OTHERGOALS),)

# this is the (weird) way of defining line breaks in Makefiles please do not
# modify
define n


endef

$(error You specified 'clean' target along with '$(OTHERGOALS)' target(s).$n \
 Please note this is incorrect as 'clean' will remove objects the '$(OTHERGOALS)' target(s) need.$n \
 Please rewrite your command like this 'make clean && make $(OTHERGOALS)')


endif

else
# the rest of the makefile can go parallel

# if application is not supposed to be boot able disable moviconvert call and mvcmd file generation, by removing from required target.
ifeq ($(MV_DISABLE_APP_BOOT_CAPABILITY),yes)
MvCmdfile :=
$(info " == Please note that this application can not be booted ! ==")
endif

export ECHO:=$(ECHO)

SET_SECTION = --set-section-flags
SECTION_FLAGS = contents,alloc,load,data

ELF_FILE = $(DirAppOutput)/$(APPNAME).elf
MAP_FILE = $(DirAppOutput)/$(APPNAME).map
ListOfOutputFolderLinkerScripts = $(patsubst %.src.ld, $(DirAppOutput)/%.ldscript, $(ListOfLinkerScriptSrcFiles))

MV_AUTO_LST ?=yes
ifeq ("$(MV_AUTO_LST)","yes")
BINARY_LST_TARGET-yes=lst
endif

#For external users we have an extra check here that we must do to ensure they have MV_TOOLS_DIR set
#as environment variable
ifeq ($(CHECK_TOOLS_VAR),yes)

CHECKINSTALLOUTPUT = $(shell cd $(MV_COMMON_BASE)/utils && ./mincheck.sh)

export ECHO:=$(ECHO)

all :
	@echo $(CHECKINSTALLOUTPUT)
ifeq ($(CHECKINSTALLOUTPUT),MV_TOOLS_DIR set)
	+$(MAKE) trueall
endif
#Output files and listings files
.PHONY: trueall
trueall :
	$(ECHO) ${MAKE} all_binaries || ${MAKE} delete_elf

.PHONY: all_binaries
all_binaries : print_configuration  $(ELF_FILE) $(MvCmdfile) $(BINARY_LST_TARGET-yes) $(DirAppOutput)/$(APPNAME).sym
	@echo "Finished building $(MV_BUILD_CONFIG) configuration for $(MV_SOC_REV)"

else
MV_PR_LOCAL_TOOLS_EXIST = $(shell if [ ! -d "$(MV_TOOLS_DIR)/$(MV_TOOLS_VERSION)" ];then echo 0;else echo 1;fi)
.PHONY: all
all :
	$(ECHO) ${MAKE} all_binaries || ${MAKE} delete_elf

.PHONY: all_binaries
all_binaries : checktools_locally print_configuration $(ELF_FILE) $(MvCmdfile) $(BINARY_LST_TARGET-yes) $(DirAppOutput)/$(APPNAME).sym
	@echo "Finished building '${MV_BUILD_CONFIG}' configuration for $(MV_SOC_REV)"

.DEFAULT_GOAL = all

# Check if tools exist, output files and listings
checktools_locally :
ifeq ($(MV_PR_LOCAL_TOOLS_EXIST), 0)
	$(error "You do not have $(MV_TOOLS_VERSION) tools locally.")
endif

print_configuration:
	@echo "Building '${MV_BUILD_CONFIG}' configuration for ${MV_SOC_REV}"

endif

getToolsVersion:
	@echo "Movidius Tools version "$(MV_TOOLS_VERSION)
	@echo "---  Shave compiler version ------"
	@$(MVCC) --version
	@echo "---  Leon compiler version -------"
	@$(CC) --version
	@echo "---  Movidius Simulator version --"
	@$(MVSIM) -version
	@echo "---  Movidius Debugger version ---"
	@$(MVDBG)  --version


CCOPT_SRC_LD= -E -x c
CCOPT_SRC_LD+= -x assembler-with-cpp
CCOPT_SRC_LD+=$(sort $(filter -DMA%,$(CCOPT) $(CCOPT_LRT) $(CCOPT_LNN)))
CCOPT_SRC_LD+=$(sort $(filter -DDEFAULT%,$(CCOPT) $(CCOPT_LRT) $(CCOPT_LNN)))
CCOPT_SRC_LD+=$(sort $(filter -DSHV_DEFAULT_STACK_SIZE%,$(CCOPT) $(CCOPT_LRT) $(CCOPT_LNN)))

define LdScriptGeneratorRules_template
.ONESHELL:
$$(DirAppOutput)/$(1).ldscript : $$(DirLDScrCommon)/$(1).src.ld
		@echo "Generate_Linker_Script : $$(<F) -> $$(call RESOLVE_TARGET_PATH,$$@) , relative path:$$@"
		@mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$(ELF_FILE)))
		$$(ECHO) $$(CXX) $$(CCOPT_SRC_LD) $$< | grep -v '^#' | sed -e 's/nl/\n/g' > $$@

endef

$(foreach file,$(LinkSrcLdFilesList),$(eval $(call LdScriptGeneratorRules_template,$(file))))

ifeq ($(MV_CPU_ARCH),sparc)
	HEADERS = $(LEON_HEADERS)
else ifeq ($(MV_CPU_ARCH),armv8)
	HEADERS = $(ARMV8_HEADERS)
endif

delete_elf :
	@echo "Removing ELF file '$(ELF_FILE)'"
	$(foreach item,$(ELF_FILE),$(call SAFE_RM,$(item)))
	exit 1 # take care to fail the overall build as this target is called upon failure

LEON_ALL_OBJECTS=$(sort $(LEON_APP_OBJECTS_REQUIRED) $(LEON_SHARED_OBJECTS_REQUIRED))
ARMV8_ALL_OBJECTS=$(sort $(ARMV8_APP_OBJECTS_REQUIRED) $(ARMV8_SHARED_OBJECTS_REQUIRED))

ifeq ($(LEON_RT_BUILD),yes)
	DefaultSparcRTEMSLibsLRTSymRename=$(subst .a,.a.lrt, $(subst .o,.o.lrt, $(DefaultSparcRTEMSLibsRT)))
	ifeq ($(MV_LOCAL_OBJ),yes)
	   DirRtemsLRTAppOutput = $(DirAppOutput)/obj_base/rtems/
	else
	   DirRtemsLRTAppOutput =
	endif
    DefaultSparcRTEMSLibsLRTSymRenameOutputDir = $(patsubst %.o.lrt, $(DirRtemsLRTAppOutput)%.o.lrt, \
                                                 $(patsubst %.a.lrt, $(DirRtemsLRTAppOutput)%.a.lrt, \
                                                 $(notdir $(DefaultSparcRTEMSLibsLRTSymRename))))
endif

.ONESHELL:
%.a.lrt : %.a
	@test -d $(DirRtemsLRTAppOutput) || mkdir -p $(DirRtemsLRTAppOutput)
	$(ECHO) $(OBJCOPY) --prefix-alloc-sections=.lrt --prefix-symbols=lrt_ $< $(DirRtemsLRTAppOutput)$(notdir $@)
.ONESHELL:
%.o.lrt : %.o
	@test -d $(DirRtemsLRTAppOutput) || mkdir -p $(DirRtemsLRTAppOutput)
	$(ECHO) $(OBJCOPY) --prefix-alloc-sections=.lrt --prefix-symbols=lrt_ $< $(DirRtemsLRTAppOutput)$(notdir $@)

ifeq ($(MV_CPU_ARCH),sparc)
.ONESHELL:
$(DirAppOutput)/$(APPNAME).map $(ELF_FILE) : mvboardscheck \
		$(LEON_RT_APPS) $(LEON_NN_APPS) $(LEON_ALL_OBJECTS) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(ARM_ELF)\
		$(ALL_SHAVE_DYN_APPS) $(SHVDYNSCRIPT) $(AllLibs) $(LinkerScript) $(ListOfOutputFolderLinkerScripts) $(DefaultSparcRTEMSLibsLRTSymRename)
	@mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$(ELF_FILE)))
	@echo "Application ELF       : $(call RESOLVE_TARGET_PATH,$@)"
	@echo "Application MAP File  : $(DirAppOutput)/$(APPNAME).map"
	@echo "Application Linker Script  : $(LinkerScript)"
	$(ECHO)$(LD) -L $(DirAppOutput) $(LDOPT) -o $(call RESOLVE_TARGET_PATH,$(ELF_FILE)) $(MV_RTEMS_PROLOGUE) $(LEON_RT_APPS) $(LEON_NN_APPS) $(LEON_ALL_OBJECTS) $(AllLibs) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(ALL_SHAVE_DYN_APPS) --start-group $(DefaultSparcRTEMSLibs) $(DefaultSparcRTEMSLibsLRTSymRenameOutputDir) --end-group  $(DefaultSparcGccLibs) $(MV_RTEMS_EPILOGUE) -Map $(DirAppOutput)/$(APPNAME).map
	# set *critical.irq_data section types to allow section init on load
	$(ECHO)$(OBJCOPY) --set-section-flags .lrt.critical.irq.data=contents,alloc,load,data \
	                  --set-section-flags .critical.irq.data=contents,alloc,load,data $(ELF_FILE)
else ifeq ($(MV_CPU_ARCH),armv8)
.ONESHELL:
$(MAP_FILE) $(ELF_FILE) : \
         $(ARMV8_ALL_OBJECTS) $(AllLibs) $(ARMV8LinkerScriptSource)
	@echo "Application ELF       : $(call RESOLVE_TARGET_PATH,$@)"
	@echo "Application Linker Script  : $(ARMV8LinkerScriptSource)"
	@echo "Application MAP File  : $(DirAppOutput)/$(APPNAME).map"
	@mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$(ELF_FILE)))
	@echo "$(LD) $(LDOPT) $(ARMV8LinkerScriptSource) -o $(call RESOLVE_TARGET_PATH,$(ELF_FILE)) "
	$(ECHO) $(LD) $(LDOPT) $(ARMV8LinkerScriptSource) -o $(call RESOLVE_TARGET_PATH,$(ELF_FILE)) \
   	$(ARMV8_ALL_OBJECTS) \
    $(AllLibs) \
		    $(DefaultARMv8GccLibs) > $(MAP_FILE)
	@echo "Application MAP File  : $(MAP_FILE)"
endif


.ONESHELL:
$(DirAppObjBase)%.o : %.c $(HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "CC   : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(CCOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%.o: %.cpp $(HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Leon CPP  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CXX) -c -fno-rtti -fno-exceptions $(CPPONLY_OPT) $(filter-out -Werror-implicit-function-declaration,$(CCOPT)) $(CPPOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%.o : %.S $(HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "ASM  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CCOPT) -DASM $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

###################################################################
#   Shave Build Rules for all common shave code
###################################################################

ifeq ($(MVCC_SKIP_ASM),yes)

ifeq ($(MVCC_DISABLE_CODE_ANALYSER),yes)
 MV_DISABLE_ANALYSER ?=,-a
endif

# The following line may be optionally enabled to see the
# invocation of moviAsm within moviCompile
#MVCC_DEBUGFLAGS=-v -save-temps

# The following assembly options are passed to the assembler to match the pre-existing settings used by the asmgen flow
# In the medium term effors should be made to remove the need for these flags to the assemble
MVCCASMOPT ?= -Wa,-noFinalSlotCompression,-i:$(MV_COMMON_BASE)/swCommon/shave_code/myriad2/include,-i:$(MV_COMMON_BASE)/swCommon/shave_code/myriad2/asm$(MV_DISABLE_ANALYSER)

.ONESHELL:
$(DirAppObjBase)%_shave.o : %.c $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave CC  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) $(MVCCASMOPT) $(MVCC_DEBUGFLAGS) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

.ONESHELL:
$(DirAppObjBase)%_shave.o : %.cpp $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave CPP : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) $(MVCCASMOPT) $(MVCC_DEBUGFLAGS) $(MVCCPPOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  $(DUMP_NULL)

else
.ONESHELL:
$(DirAppObjBase)%_shave.o : $(DirAppObjBase)%.asmgen $(SHAVE_ASM_HEADERS) Makefile
	@echo "Shave ASMGEN  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%.asmgen : %.c $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave CC  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

.ONESHELL:
$(DirAppObjBase)%.asmgen : %.cpp $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave CPP : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) $(MVCCPPOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  $(DUMP_NULL)

#Adding SHAVE_NN rules
.ONESHELL:
$(DirAppObjBase)%_shaveNN.o : $(DirAppObjBase)%.asmgenNN $(SHAVE_ASM_HEADERS) Makefile
	@echo "Shave NN ASMGEN  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%.asmgenNN : %.c $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave NN CC  : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCC_NN_OPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

.ONESHELL:
$(DirAppObjBase)%.asmgenNN : %.cpp $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave NN CPP : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCC_NN_OPT) $(MVCCPP_NN_OPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  $(DUMP_NULL)


endif

.ONESHELL:
$(DirAppObjBase)%_shave.o : %.asm $(SHAVE_ASM_HEADERS) Makefile
	@echo "Shave ASM : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT) $(mvAsmAsmfilesOpt)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@)

#And SHAVE_NN rules
.ONESHELL:
$(DirAppObjBase)%_shaveNN.o : %.asm $(SHAVE_ASM_HEADERS) Makefile
	@echo "Shave NN ASM : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT) $(mvAsmAsmfilesOpt)  $(call RESOLVE_TARGET_PATH,$<) -o:$(call RESOLVE_TARGET_PATH,$@)


.ONESHELL:
$(DirAppObjBase)%.i : %.c $(SHAVE_C_HEADERS) Makefile
	@echo "Shave pre-process c   : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) -E $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)

.ONESHELL:
$(DirAppObjBase)%.ipp : %.cpp $(SHAVE_C_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Shave pre-process c++ : $(call RESOLVE_TARGET_PATH,$<)"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(MVCC) $(MVCCOPT) $(MVCCPPOPT) -E $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) $(DUMP_NULL)


#########################################################################
#########################################################################

#Move section prefixes for shave0 selected applications. This is the section describing
#ABSOLUTE relocations
#That filter line there is just taking out the application name by removing the ".mvlib" extension
#and the leading words

.SECONDEXPANSION:
define MOVE_SHAVE_PREFIX_FOR_LIB

$(2).shv$(1)lib : $(2).shv$(1)templib
	$$(ECHO) $$(OBJCOPY) -g --redefine-sym $$(lastword $$(filter-out shv%templib,$$(subst /, ,$$(subst ., ,$$<))))$(1)___SglResMgrGlobal=__SglResMgrGlobal -W __SglResMgrGlobal  --redefine-sym $$(lastword $$(filter-out shv%templib,$$(subst /, ,$$(subst ., ,$$<))))$(1)_mvShavePipePrintQueueGet=mvShavePipePrintQueueGet --redefine-sym $$(lastword $$(filter-out shv%templib,$$(subst /, ,$$(subst ., ,$$<))))$(1)_mvShavePipePrintQueueAdd=mvShavePipePrintQueueAdd --redefine-sym $$(lastword $$(filter-out shv%templib,$$(subst /, ,$$(subst ., ,$$<))))$(1)____globalTail=___globalTail -W ___globalTail --redefine-sym $$(lastword $$(filter-out shv%templib,$$(subst /, ,$$(subst ., ,$$<))))$(1)__globalShaveData=__globalShaveData -W __globalShaveData $$< $$(call RESOLVE_TARGET_PATH,$$@)

#And one section to place contents in windowed addresses if we so choose
$(2).shv$(1)wndlib : $(2).mvlib
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.wndshv$(1). --prefix-symbols=$$(call basenamenotdir,$$<)$(1)_ $$< $$(call RESOLVE_TARGET_PATH,$$@)

endef

define MOVE_SHAVE_PREFIX

%.shv$(1)lib : %.shv$(1)templib
	$$(ECHO) $$(OBJCOPY) --redefine-sym $$(call basenamenotdir,$$<)$(1)___SglResMgrGlobal=__SglResMgrGlobal -W __SglResMgrGlobal  --redefine-sym $$(call basenamenotdir,$$<)$(1)_mvShavePipePrintQueueGet=mvShavePipePrintQueueGet --redefine-sym $$(call basenamenotdir,$$<)$(1)_mvShavePipePrintQueueAdd=mvShavePipePrintQueueAdd --redefine-sym $$(call basenamenotdir,$$<)$(1)____globalTail=___globalTail -W ___globalTail --redefine-sym $$(call basenamenotdir,$$<)$(1)__globalShaveData=__globalShaveData -W __globalShaveData  $$< $$(call RESOLVE_TARGET_PATH,$$@)
%.shv$(1)templib : %.mvlib
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.shv$(1). --prefix-symbols=$$(call basenamenotdir,$$<)$(1)_ $$< $$(call RESOLVE_TARGET_PATH,$$@)

#And one section to place contents in windowed addresses if we so choose
%.shv$(1)wndlib : %.mvlib
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.wndshv$(1). --prefix-symbols=$$(call basenamenotdir,$$<)$(1)_ $$< $$(call RESOLVE_TARGET_PATH,$$@)

endef

$(foreach idx,$(SHAVES_IDX),$(eval $(call MOVE_SHAVE_PREFIX,$(idx))))

#and do the same for SHAVE_NN libraries
define MOVE_SHAVE_NN_PREFIX

%.shvNN$(1)lib : %.shvNN$(1)templib
	$$(ECHO) $$(OBJCOPY) --redefine-sym $$(call basenamenotdir,$$<)$(1)___SglResMgrGlobal=__SglResMgrGlobal -W __SglResMgrGlobal  --redefine-sym $$(call basenamenotdir,$$<)$(1)_mvShavePipePrintQueueGet=mvShavePipePrintQueueGet --redefine-sym $$(call basenamenotdir,$$<)$(1)_mvShavePipePrintQueueAdd=mvShavePipePrintQueueAdd --redefine-sym $$(call basenamenotdir,$$<)$(1)____globalTail=___globalTail -W ___globalTail $$< $$(call RESOLVE_TARGET_PATH,$$@)
%.shvNN$(1)templib : %.mvlib
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.shvNN$(1). --prefix-symbols=$$(call basenamenotdir,$$<)$(1)_ $$< $$(call RESOLVE_TARGET_PATH,$$@)

#And one section to place contents in windowed addresses if we so choose
%.shvNN$(1)wndlib : %.mvlib
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.wndshvNN$(1). --prefix-symbols=$$(call basenamenotdir,$$<)$(1)_ $$< $$(call RESOLVE_TARGET_PATH,$$@)

endef

$(foreach idx,$(SHAVES_IDX),$(eval $(call MOVE_SHAVE_NN_PREFIX,$(idx))))

ifeq ($(SHAVE_APP_ENABLE_DEBUG_CONTEXT), YES)
#Targets for dynamically loadable objects
%_temp.shvdlib : %.mvlib
	@echo "Creating dynamically loadable library : $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(LDDYNOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  -Map $(call RESOLVE_TARGET_PATH,$@).map

%.shvdlib : %_temp.shvdlib
	@NAME_LEN=$$(( $$(wc -m <<< $(subst _temp.shvdlib,,$(notdir $<))) - 1 ))
	ALLIGN_LEN=$$(( 4 - ($$NAME_LEN - ($$NAME_LEN  / 4)*4)))  #this line is used for Ubuntu 14 compatibility where shell can not calculate modulo op.
	# ALLIGN_LEN=$$(( 4 - $$NAME_LEN  % 4))
	printf "%04d" $$NAME_LEN > $@
	printf "%s" $(subst _temp.shvdlib,,$(notdir $<)) >> $@
	if [ "$$ALLIGN_LEN" != "4" ]; then
		for (( i=1; i <= $$ALLIGN_LEN; ++i ))
		do
			printf "%d" 0 >> $@
		done
	fi
	cat $< >> $@

else
#Targets for dynamically loadable objects
%.shvdlib : %.mvlib
	@echo "Creating dynamically loadable library : $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(LDDYNOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  -Map $(call RESOLVE_TARGET_PATH,$@).map

endif

#this gives out all symbols too
%.shvdcomplete : %.mvlib
	@echo "Creating windowed library for symbol extraction $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(LDSYMOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)  -Map $(call RESOLVE_TARGET_PATH,$@).map

#this creates the required symbols file with only the symbols for the dynamic section
%_sym.o : %.shvdcomplete
	@echo "Creating symbols file for the dynamic section $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(OBJCOPY) --prefix-symbols=$(call basenamenotdir,$<)_ --extract-symbol $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

#Some generic targets for raw objects
%_raw.o : %.shvdlib
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=$(call basenamenotdir,$<)Mbin \
	-O elf32-littlesparc -B sparc $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

# General rule to make MVcmd from a elf, only if file name defined
ifneq ($(MvCmdfile),)
.ONESHELL:
$(DirAppOutput)/%.mvcmd : $(DirAppOutput)/%.elf
	@echo "MVCMD boot image      : $(call RESOLVE_TARGET_PATH,$@)"
	@mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$@))
ifeq ($(MV_CPU_ARCH), sparc)
	$(ECHO) $(MVCONV) $(MVCONVOPT) $(MVCMDOPT) $(^) -mvcmd:$(@)  $(DUMP_NULL)
else ifeq ($(MV_CPU_ARCH), armv8)
ifeq ($(MV_CPU_ARCH_64BIT), yes)
	@echo "ARMv8 64-bit  MoviConvert"
	$(ECHO) $(MVCONV64) -cv:ma2480 -hexConfig:$(DirLDScrCommonPP)/myriad2collection/arch/ma2490/Myriad2HexConfig_LE.cfg $(^) -hex:output
else
	@echo "ARMv8 MoviConvert"
	$(MVCONV) -cv:ma2480 -hexConfig:$(DirLDScrCommonPP)/myriad2collection/arch/ma2490/Myriad2HexConfig_LE.cfg $(^) -hex:output
endif
else
   $$(error Unsupported MV_CPU_ARCH for OS core: $$(MV_CPU_ARCH))
endif
endif

ifeq ($(DefaultLinkerGenerateRule),yes)
.ONESHELL:
$(LinkerScript) : $(LinkerScriptSource)
ifneq ($(LinkerScriptSource),)
	@echo "Generate Linker Script: $(<F) -> $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(CC) -E -P -C -D$(MV_SOC_REV)			     \
	       -I $(DirAppRoot)/scripts					     \
	       -I $(DirLDScript)					     \
	       -I $(DirLDScrCommonPP)/$(MV_SOC_PLATFORM)		     \
	       -I $(DirLDScrCommon)/$(MV_SOC_PLATFORM) $(LinkerScriptSource) \
	       -o $(LinkerScript)
endif
endif

$(ARMV8LinkerScript) : $(ARMV8LinkerScriptSource)
###################################################################
# Shave dynamic loading specific rules
###################################################################

###################################################################
#  START of Group handling part
###################################################################
define cGroupRules_template
#Get list of symbols to mark -u based on all apps using this library group
.ONESHELL:
$$($(1)).grpsyms : $$(addsuffix .mvlib,$$(MV_$(1)_APPS))
	@echo "Creating list of symbols to be trapped from the library group $$@"
	test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(READELF) --syms --wide $$(addsuffix .mvlib,$$(MV_$(1)_APPS)) | grep -v Symbol | grep -v Vis | grep -v File | grep -v FILE | grep UND | sed 's/\s\s*/ /g' | cut -d ' ' -f 9 | { grep . || true;} | /usr/bin/sort -n | uniq >> $$(call RESOLVE_TARGET_PATH,$$@)
	@echo "Adding the AllTimeEntry point to the symbols to take from the group $$@"
	$$(ECHO) echo ___AllTimeEntryPoint >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo _EP_crtfini >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo _EP_crtinit >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo _EP_start >> $$(call RESOLVE_TARGET_PATH,$$@)

#Get list of all possible symbols provided by the Group
$(1)_FILT_LIBS=$$(filter-out --start-group --end-group --whole-archive --no-whole-archive,$$($(1)_LIBS))
.ONESHELL:
#Append __fini_array_start __fini_array_end to make sure they get translated even if empty
#Append __init_array_start __init_array_end to make sure they get translated even if empty
$$($(1)).grpallsyms : $$($(1)_FILT_LIBS)
	@echo "Generate list of all symbols defined by the group $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(READELF) --syms --wide $$($(1)_FILT_LIBS) | grep -v Symbol | grep -v Vis | grep -v File | grep -v FILE | grep -v UND | sed 's/\s\s*/ /g' | cut -d ' ' -f 9 | { grep . || true;} | /usr/bin/sort -n | uniq  > $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo __fini_array_start >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo __fini_array_end >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo __init_array_start >> $$(call RESOLVE_TARGET_PATH,$$@)
	$$(ECHO) echo __init_array_end >> $$(call RESOLVE_TARGET_PATH,$$@)


#Group symbols to redefine. Make sure to take out of the redefinition rules the globalTail and ResMgr
.ONESHELL:
$$($(1)).grpredefinesyms : $$($(1)).grpallsyms
	@echo "Generate redefinitions for groups $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) cat $$< | /usr/bin/sort -n | uniq  | grep -v "__SglResMgrGlobal\|__FilterResourceManagerGlobal\|__hwFilterListGlobal\|___globalTail" | sed 's/.*/& $(1)_&/g'> $$@

.ONESHELL:
$$($(1)).lnkcmds_tmp : $$($(1)).grpsyms $$($(1)_FILT_LIBS)
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) echo $$(LdCommandLineFileHeader) --start-group $$($(1)_LIBS) --end-group > $$@
	$$(ECHO) cat $$< | /usr/bin/sort -n | uniq | sed 's/.*/-u &/g' >> $$@

$$($(1)).lnkcmds : $$($(1)).lnkcmds_tmp
	$$(ECHO) tr '\n' ' ' < $$< > $$@

.ONESHELL:
$$($(1)).mvlibG : $$($(1)).lnkcmds $$($(1)_FILT_LIBS)
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(LD) @$$< -o $$(call RESOLVE_TARGET_PATH,$$@) -Map $$(call RESOLVE_TARGET_PATH,$$@).map

#Creating library group and making sure the global ResMgr and globalTail symbols are weakened so that the Leon ones are strongest
.ONESHELL:
$$($(1)).libgroup : $$($(1)).mvlibG $$($(1)).grpredefinesyms
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.shvZ.$(1). --redefine-syms=$$($(1)).grpredefinesyms -W __SglResMgrGlobal -W __FilterResourceManagerGlobal -W ___globalTail -W __hwFilterListGlobal\
	$$< $$(call RESOLVE_TARGET_PATH,$$@)

.ONESHELL:
$$($(1)).shvZdata : $$($(1)).mvlibG
	@echo "Creating dynamically loadable library group data : $$@"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(LD) $$(LD_ENDIAN_OPT) $$(V_GRP_LDDYNOPT) $$< -o $$@ -Map $$@.map

ifeq ($$(SHAVE_APP_ENABLE_DEBUG_CONTEXT), YES)
.ONESHELL:
$$($(1))_temp.shvZdata : $$($(1)).shvZdata
	@NAME_LEN=$$$$(( $$$$(wc -m <<< $$(subst .shvZdata,,$$(notdir $$<))) - 1 ))
	ALLIGN_LEN=$$$$(( 4 - ($$$$NAME_LEN - ($$$$NAME_LEN  / 4)*4)))   #this line is used for Ubuntu 14 compatibility where shell can not calculate modulo op.
	# ALLIGN_LEN=$$$$(( 4 - $$$$NAME_LEN  % 4))
	printf "%04d" $$$$NAME_LEN > $$@
	printf "%s" $$(subst .shvZdata,,$$(notdir $$<)) >> $$@
	if [ "$$$$ALLIGN_LEN" != "4" ]; then
		for (( i=1; i <= $$$$ALLIGN_LEN; ++i ))
		do
			printf "%d" 0 >> $$@
		done
	fi
	cat $$< >> $$@

.ONESHELL:
$$($(1))_shvZdata.o : $$($(1))_temp.shvZdata
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$$(subst -,_,$$(subst /,_,$$(subst .,_,$$<)))_start=$$(subst _temp.shvZdata,,$$(notdir $$<))grpdyndata \
	-O elf32-littlesparc -B sparc $$< $$@

else

.ONESHELL:
$$($(1))_shvZdata.o : $$($(1)).shvZdata
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$$(subst -,_,$$(subst /,_,$$(subst .,_,$$<)))_start=$$(subst .shvZdata,,$$(notdir $$<))grpdyndata \
	-O elf32-littlesparc -B sparc $$< $$@

endif

.ONESHELL:
$$($(1)).shvZdatacomplete : $$($(1)).mvlibG
	@echo "Creating windowed library for symbol extraction $$@"
	$$(ECHO) $$(LD) $$(LD_ENDIAN_OPT) $$(V_GRP_LDSYNOPT) $$< -o $$@

.ONESHELL:
$$($(1))_shvZdata_sym.o : $$($(1)).shvZdatacomplete
	@echo "Creating symbols file for shave group dynamic loading section $$(call RESOLVE_TARGET_PATH,$$@)"
	$$(ECHO) $$(OBJCOPY) --prefix-symbols=$$(call basenamenotdir,$$<)_ --extract-symbol $$(call RESOLVE_TARGET_PATH,$$<) $$(call RESOLVE_TARGET_PATH,$$@)

PROJECTCLEAN += $$($(1)).grpsyms $$($(1)).grpallsyms
PROJECTCLEAN += $$($(1)).lnkcmds_tmp $$($(1)).lnkcmds
PROJECTCLEAN += $$($(1)).mvlibG $$($(1)).libgrouptemp
PROJECTCLEAN += $$($(1)).libgroup $$($(1)).shvZdata
PROJECTCLEAN += $$($(1)).shvZdatacomplete $$($(1))_shvZdata.o
PROJECTCLEAN += $$($(1))_shvZdata_sym.o

endef

$(foreach group,$(MV_SHAVE_GROUPS),$(eval $(call cGroupRules_template,$(group))))

###################################################################
#  END of Group handling part
###################################################################

###################################################################
#  START of App handling part -> but still using group name to reference things
###################################################################
#The mvlib rule is available from the main application file, so starting here from that point onward

#Creation of the application dyncontext object

define DYNCONTEXT_template
$$(DirAppObjBase)$(1)_dyncontext.o : $$(MV_COMMON_BASE)/swCommon/dynamicStructure/theDynContext.c $$($(1)_GROUP_DATA) $(2).shvXdata
	@echo "Create dyncontext for the current application $@"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CC) -c $$(CONLY_OPT) $$(CCOPT) $$(cDynContextDefs_$(1)) \
	-D'APPDYNCONTEXTNAME=$(1)X_ModuleData' \
	-D'APPDYNCONTEXTPRIVATENAME=$(1)X_ModuleDataPrivate' \
	-D'APPEXECCONTEXTARRAY=$(1)X_ExecutionContext' \
	-D'APPHEAPDYNCONTEXTADR=$(1)X_heap' \
	-D'APPGRPDATADYNCONTEXTADR=$(1)X_grpData' \
	-D'CTORSARRAY=$(1)X___init_array_start' \
	-D'INITARRAYEND=$(1)X___init_array_end' \
	-D'DTORSARRAY=$(1)X___fini_array_start' \
	-D'FINIARRAYEND=$(1)X___fini_array_end' \
	-D'APPGROUPDYNDATASECTIONSIZE=0x0$$(shell $$(OBJDUMP) -xsrtd $$($(1)_GROUP_DATA) | grep dyn.data | grep -v section | cut -d ' ' -f 9)ul' \
	-D'APPDYNDATASECTIONSIZE=0x0$$(shell $$(OBJDUMP) -xsrtd $(2).shvXdata | grep dyn.data | grep -v section | grep -v : | cut -d ' ' -f 9)ul' \
	-D'APPCRITCMXTEXTSECTIONSIZE=0x0$$(shell $$(OBJDUMP) -xsrtd $(2).shvXdata | grep dyn.textCrit | grep -v section | grep -v : | cut -d ' ' -f 5)ul' \
	-D'APPIAT=$(1)__iat_array_start' \
	-D'APPIATNAMES=$(1)__iatnames_array_start' \
	-D'APPIATSTRINGS=$(1)__iatstrings_array_start' \
	$(call RESOLVE_TARGET_PATH,$$<) -o $(call RESOLVE_TARGET_PATH,$$@)

endef

$(foreach app,$(shaveXApps),$(eval $(call DYNCONTEXT_template,$(call basenamenotdir,$(app)),$(app))))

#Have a special build rule for building the global array for dyncontexts as in the future we may want to use special rules here
.ONESHELL:
$(DirAppObjBase)dynContextMaster.o : $(DirAppObjBase)dynContextMaster.c
	@echo "Create global dyncontext array for the current application $@"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(CCOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

#Create list of local application symbols
%.appsyms : %.mvlib
		$(ECHO) $(READELF) --syms --wide $< | grep -v Symbol | grep -v Vis | grep -v File | grep -v FILE | grep -v UND | sed 's/\s\s*/ /g' | cut -d ' ' -f 9 | grep . | /usr/bin/sort -n | uniq  >> $(call RESOLVE_TARGET_PATH,$@)

#App symbols to redefine. Make sure to take out the global __SglResMgrGlobal, __FilterResourceManagerGlobal, __hwFilterListGlobal and ___globalTail symbols, just in case they get their way in the .shvXlib too
%.appredefinesyms : %.appsyms
	$(ECHO) cat $< | /usr/bin/sort -n | uniq | grep -v "__SglResMgrGlobal\|__hwFilterListGlobal\|__FilterResourceManagerGlobal\|___globalTail" | sed 's/.*/& $(call basenamenotdir,$<)X_&/g'> $(call RESOLVE_TARGET_PATH,$@)

define cGroupAppXLibRules_template
#Create a shvXlib*redefsyms file which is a collection of appredefine syms and grpredefinesyms
#To take note: the main purpose this is a separate rule is to handle linkonce symbols. These
#are for example static functions declared in headers: they are allowed but when header is included in different
#C files, they will be linked twice. In C this may look slightly odd but in C++ this is actually very much used
#because many libraries wil declare template classes in headers for example
#So what we are doing is: we are keeping the local application present linkonce data.
#"Why not the library one?" -> one might ask. Because the library one is a mirror of whoever
#built the libraries, and they may have build them with flags different than your own apps. So:
#staying with the application one. Note: this doesn't affect cases where the library itself has a need
#to call it's own linkonce cases because this does not affect libgroup creation
%.shvXlib$(1)redefsyms : %.mvlib $$(addsuffix .appredefinesyms,$$(MV_$(1)_APPS)) $$($(1)).grpredefinesyms
	$$(ECHO) cat $$($(1)).grpredefinesyms $$(patsubst %.mvlib,%.appredefinesyms,$$<) | \
	sed 's/$$(call basenamenotdir,$$<)/AA$$(call basenamenotdir,$$<)/g' \
	| sed 's/$(1)/ZZ$(1)/g' \
	| sort | rev | uniq -f 1 | rev | sed 's/ZZ$(1)/$(1)/g' \
	| sed 's/AA$$(call basenamenotdir,$$<)/$$(call basenamenotdir,$$<)/g' > $$(call RESOLVE_TARGET_PATH,$$@)


#shvXlib creation by using the redefine syms list for the current application and the ones from the group. Make sure the GlobalTail and SglResMgrGlobal are weakened
.ONESHELL:
%.shvXlib$(1) : %.shvXlib$(1)redefsyms
	@echo "Moving SHAVE application to its own group application type $$@"
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.shvX.$$(call basenamenotdir,$$<). \
	--redefine-syms=$$(call RESOLVE_TARGET_PATH,$$<) \
	-W __SglResMgrGlobal -W __FilterResourceManagerGlobal -W ___globalTail -W __hwFilterListGlobal \
	$$(patsubst %.shvXlib$(1),%.mvlib,$$@) $$(call RESOLVE_TARGET_PATH,$$@)
endef

$(foreach group,$(MV_SHAVE_GROUPS),$(eval $(call cGroupAppXLibRules_template,$(group))))

#Get the Application data
.ONESHELL:
%.shvXdata : %.mvlib
	@echo "Creating dynamically loadable shave application data : $@"
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(V_APP_LDDYNOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) -Map $(call RESOLVE_TARGET_PATH,$@).map


ifeq ($(SHAVE_APP_ENABLE_DEBUG_CONTEXT), YES)
.ONESHELL:
%_temp.shvXdata : %.shvXdata
	@NAME_LEN=$$(( $$(wc -m <<< $(subst .shvXdata,,$(notdir $<))) - 1 ))
	ALLIGN_LEN=$$(( 4 - ($$NAME_LEN - ($$NAME_LEN  / 4)*4)))  #this line is used for Ubuntu 14 compatibility where shell can not calculate modulo op.
	# ALLIGN_LEN=$$(( 4 - $$NAME_LEN  % 4))
	printf "%04d" $$NAME_LEN > $@
	printf "%s" $(subst .shvXdata,,$(notdir $<)) >> $@
	if [ "$$ALLIGN_LEN" != "4" ]; then
		for (( i=1; i <= $$ALLIGN_LEN; ++i ))
		do
			printf "%d" 0 >> $@
		done
	fi
	cat $< >> $@

.ONESHELL:
%_shvXdata.o : %_temp.shvXdata
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=$(subst _temp.shvXdata,,$(notdir $<))appdyndata \
	-O elf32-littlesparc -B sparc $< $(call RESOLVE_TARGET_PATH,$@)

else

.ONESHELL:
%_shvXdata.o : %.shvXdata
	$(ECHO) $(OBJCOPY)  -I binary --rename-section .data=.ddr.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=$(subst .shvXdata,,$(notdir $<))appdyndata \
	-O elf32-littlesparc -B sparc $< $(call RESOLVE_TARGET_PATH,$@)

endif

.ONESHELL:
%.shvXdatacomplete : %.mvlib
	@echo "Creating windowed library for symbol extraction of loadable shave application $@"
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(V_APP_LDSYNOPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@) -Map $(call RESOLVE_TARGET_PATH,$@).map

.ONESHELL:
%_shvXdata_sym.o : %.shvXdatacomplete
	@echo "Creating symbols file for shave app dynamic loading section $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(OBJCOPY) --prefix-symbols=$(call basenamenotdir,$<)_ --extract-symbol $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

#Add list of current group apps for cleaning
define cGroupAppXLibCleanRules_template
PROJECTCLEAN += $$(addsuffix .appsyms,$$(MV_$(1)_APPS))
PROJECTCLEAN += $$(addsuffix .appredefinesyms,$$(MV_$(1)_APPS))
PROJECTCLEAN += $$(addsuffix .shvXlib$(1),$$(MV_$(1)_APPS))
endef

$(foreach group,$(MV_SHAVE_GROUPS),$(eval $(call cGroupAppXLibCleanRules_template,$(group))))

PROJECTCLEAN += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix .shvXdata,$(MV_$(group)_APPS)))
PROJECTCLEAN += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix .shvXdatacomplete,$(MV_$(group)_APPS)))
PROJECTCLEAN += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix _shvXdata.o,$(MV_$(group)_APPS)))
PROJECTCLEAN += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix _shvXdata_sym.o,$(MV_$(group)_APPS)))
PROJECTCLEAN += $(foreach group,$(MV_SHAVE_GROUPS),$(addsuffix _dyncontext.o,$(MV_$(group)_APPS)))

###################################################################
#  END of App handling part
###################################################################

ifneq ($(strip $(shaveXApps)),)
#Rule to create the required linkerscript for building the dynamically loaded libs
.ONESHELL:
./$(DirAppOutput)/shvDynInfrastructureBase.ldscript :
	@echo "Create custom Shave Dyn. Data Loading linker script rules $@"
	@test -d $(@D) || mkdir -p $(@D)
	for appName in $(foreach app,$(shaveXApps),$(call basenamenotdir,$(app))); \
	do cat $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadTemplate_App.ldscript | sed s/ZZZZZZZZZ/"$$appName"/g | sed s/YYYYYYYYY/"$$previous"/g >> $(DirAppOutput)/shvDynInfrastructureTemp.ldscript ; \
	previous=$$file;\
	done
	for grpName in $(foreach group,$(MV_SHAVE_GROUPS),$(call basenamenotdir,$(group))); \
	do cat $(MV_COMMON_BASE)/scripts/ld/shaveDynamicLoad/shaveDynLoadTemplate_Group.ldscript | sed s/ZZZZZZZZZ/"$$grpName"/g | sed s/YYYYYYYYY/"$$previous"/g >> $(DirAppOutput)/shvDynInfrastructureTemp.ldscript ; \
	previous=$$file;\
	done
	$(ECHO) cat $(DirAppOutput)/shvDynInfrastructureTemp.ldscript | sed s/"LOADADDR (S.lrt.shvCOM.cmx.data.Shave) + SIZEOF(S.lrt.shvCOM.cmx.data.Shave)"/"0xE0000000"/ > $(DirAppOutput)/shvDynInfrastructureBase.ldscript
	@rm -rf $(DirAppOutput)/shvDynInfrastructureTemp.ldscript
endif

###################################################################
# LeonRT Build Rules
###################################################################
# Now we have to provide crti, crtbegin, crtn and crtend for RTEMS 4.10.99 MV_RTEMS_PROLOGUE and MV_RTEMS_EPILOGUE
# As both Leons may have different compiling options they must use different objects
$(LEON_RT_LIB_NAME).mvlib : $(LEON_RT_APP_OBJS) $(LEON_SHARED_OBJECTS_REQUIRED_LRT) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(DefaultSparcRTEMSLibsRT)
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(LRT_PROJECT_LIBS) $(addprefix -L ,$(DirSparcDefaultLibs)) -Ur $(MV_RTEMS_PROLOGUE_LRT) $(LEON_RT_APP_OBJS) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(LEON_SHARED_OBJECTS_REQUIRED_LRT) --start-group  $(MV_RTEMS_EPILOGUE_LRT) --end-group $(DefaultSparcGccLibs) -o $(call RESOLVE_TARGET_PATH,$@)  -Map $(call RESOLVE_TARGET_PATH,$@).map
.ONESHELL:
$(DirAppObjBase)%_lrt.o : %.c $(LEON_HEADERS_LRT) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonRT CC : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(CCOPT_LRT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%_lrt.o: %.cpp $(LEON_HEADERS_LRT) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonRT CC : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c -fno-rtti -fno-exceptions $(filter-out -Werror-implicit-function-declaration,$(CCOPT_LRT)) $(CPPONLY_OPT) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%_lrt.o : %.S $(LEON_HEADERS_LRT) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonRT ASM: $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CCOPT_LRT) -DASM $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

###################################################################
# LeonNN Build Rules (coming up starting from VPU2 SoCs)
###################################################################
# Now we have to provide crti, crtbegin, crtn and crtend for RTEMS 4.10.99 MV_RTEMS_PROLOGUE and MV_RTEMS_EPILOGUE
# As both Leons may have different compiling options they must use different objects
$(LEON_NN_LIB_NAME).mvlib : $(LEON_NN_APP_OBJS) $(LEON_SHARED_OBJECTS_REQUIRED_LNN) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(DefaultSparcRTEMSLibsNN)
	$(ECHO) $(LD) $(LD_ENDIAN_OPT) $(LNN_PROJECT_LIBS) $(addprefix -L ,$(DirSparcDefaultLibs)) -Ur $(MV_RTEMS_PROLOGUE_LNN) $(LEON_NN_APP_OBJS) $(ALL_SHAVE_NN_APPS) $(ALL_SHAVE_APPS) $(LEON_SHARED_OBJECTS_REQUIRED_LNN) --start-group  $(MV_RTEMS_EPILOGUE_LNN) --end-group $(DefaultSparcGccLibs) -o $(call RESOLVE_TARGET_PATH,$@)  -Map $(call RESOLVE_TARGET_PATH,$@).map
.ONESHELL:
$(DirAppObjBase)%_lnn.o : %.c $(LEON_HEADERS_LNN) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonNN CC : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(CCOPT_LNN) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%_lnn.o: %.cpp $(LEON_HEADERS_LNN) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonNN CC : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CPPONLY_OPT) $(filter-out -Werror-implicit-function-declaration,$(CCOPT_LNN)) -fno-rtti -fno-exceptions  $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppObjBase)%_lnn.o : %.S $(LEON_HEADERS_LNN) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "LeonNN ASM: $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CCOPT_LNN) -DASM $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

include $(MV_COMMON_BASE)/individual_file_options.mk


ifneq ($(strip $(shaveXApps)),)
# If there are dynamically loaded applications then
# redefine symbols that have to be the same for LOS and LRT processors, by removing the "lrt_" prefix

#For each app, create the relevant .symuniq separate list
.ONESHELL:
%.lrtappsyms : %.appsyms
	@echo "Generate LeonRT symbol uniquificator for application $@"
	$(ECHO) cat $< | /usr/bin/sort -n | uniq | sed 's/.*/lrt_$(call basenamenotdir,$<)X_& $(call basenamenotdir,$<)X_&/g' > $(call RESOLVE_TARGET_PATH,$@)
	$(ECHO) echo lrt_$(call basenamenotdir,$<)appdyndata $(call basenamenotdir,$<)appdyndata >> $(call RESOLVE_TARGET_PATH,$@)
	$(ECHO) echo lrt_$(call basenamenotdir,$<)grpdyndata $(call basenamenotdir,$<)grpdyndata >> $(call RESOLVE_TARGET_PATH,$@)
	$(ECHO) echo lrt_$(call basenamenotdir,$<)X_ModuleData $(call basenamenotdir,$<)X_ModuleData >> $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
%.lrtgrpsyms : %.grpsyms
	@echo "Generate LeonRT symbol uniquificator for group $@"
	$(ECHO) cat $< | /usr/bin/sort -n | uniq | sed 's/.*/lrt_$(call basenamenotdir,$<)_& $(call basenamenotdir,$<)_&/g' > $(call RESOLVE_TARGET_PATH,$@)
	$(ECHO) echo lrt_$(call basenamenotdir,$<)grpdyndata $(call basenamenotdir,$<)grpdyndata >> $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
$(DirAppOutput)/shaveXApps.symuniq : $(addsuffix .lrtappsyms,$(foreach group,$(MV_SHAVE_GROUPS),$(MV_$(group)_APPS))) $(addsuffix .lrtgrpsyms,$(foreach group,$(MV_SHAVE_GROUPS),$($(group))))
	@echo "Generate LeonRT master symbol uniquificator $@"
	@test -d $(@D) || mkdir -p $(@D)
	cat $(addsuffix .lrtappsyms,$(foreach group,$(MV_SHAVE_GROUPS),$(MV_$(group)_APPS))) > $(call RESOLVE_TARGET_PATH,$@)
	cat $(addsuffix .lrtgrpsyms,$(foreach group,$(MV_SHAVE_GROUPS),$($(group)))) >> $(call RESOLVE_TARGET_PATH,$@)

RTLIB_EXTRA_DEPS=$(DirAppOutput)/shaveXApps.symuniq
NNLIB_EXTRA_DEPS=$(DirAppOutput)/shaveXApps.symuniq

UNIQUE_SYMBOLS+= GlobalContextData
UNIQUE_SYMBOL_RULES+= --redefine-syms=$(DirAppOutput)/shaveXApps.symuniq

endif

UNIQUE_SYMBOLS+= __SglResMgrGlobal mvShavePipePrintQueueAdd mvShavePipePrintQueueGet ___globalTail __globalShaveData __MemManager __hwFilterListGlobal
UNIQUE_SYMBOL_RULES+= $(foreach sym,$(UNIQUE_SYMBOLS),--redefine-sym lrt_$(sym)=$(sym) -W $(sym))

# Redefine symbols that have to be the same for LOS and LRT processors, by removing the "lrt_" prefix
%.rtlib : %.rtlibtemp $(RTLIB_EXTRA_DEPS)
	$(ECHO) $(OBJCOPY) $(UNIQUE_SYMBOL_RULES) $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
%.rtlibtemp : %.mvlib
	@echo "Generating unique instance of leon_nn application: $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(OBJCOPY) --prefix-alloc-sections=.lrt --prefix-symbols=lrt_ $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

# Redefine symbols that have to be the same for LOS and LRT and LNN processors, by removing the "lnn_" prefix
%.nnlib : %.nnlibtemp $(NNLIB_EXTRA_DEPS)
	$(ECHO) $(OBJCOPY) $(UNIQUE_SYMBOL_RULES) $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)

.ONESHELL:
%.nnlibtemp : %.mvlib
	@echo "Generating unique instance of leon_nn application: $(call RESOLVE_TARGET_PATH,$@)"
	$(ECHO) $(OBJCOPY) --prefix-alloc-sections=.lnn --prefix-symbols=lnn_ $(call RESOLVE_TARGET_PATH,$<) $(call RESOLVE_TARGET_PATH,$@)


endif # ifneq ($(filter clean,$(MAKECMDGOALS))

###################################################################
#       Including some specific build rules we need for all       #
###################################################################
-include $(MV_COMMON_BASE)/setupbinaries.mk
include $(MV_COMMON_BASE)/commonbuilds.mk
include $(MV_COMMON_BASE)/mbinflow.mk
include $(MV_COMMON_BASE)/functional_targets.mk
-include $(MV_COMMON_BASE)/inhousetargets.mk

include_pwr_manager =
ifeq ($(findstring PwrManager,$(ComponentList_LOS)),PwrManager)
    include_pwr_manager = yes
endif
ifeq ($(findstring PwrManager,$(ComponentList)),PwrManager)
    $(error PwrManager cannot be used in LRT)
endif
ifeq ($(findstring PwrManager,$(ComponentList_LRT)),PwrManager)
    $(error PwrManager cannot be used in LRT)
endif

ifeq ($(findstring SuspendLrt,$(ComponentList_LRT)),SuspendLrt)
    CCOPT += -DUSE_SUSPEND_LRT
endif
ifeq ($(findstring SuspendLrt,$(ComponentList)),SuspendLrt)
    $(error SuspendLrt cannot be used in LOS)
endif
ifeq ($(findstring SuspendLrt,$(ComponentList_LOS)),SuspendLrt)
    $(error Suspendlrt cannot be used in LOS`
endif

ifdef include_pwr_manager
    include $(MV_COMMON_BASE)/pwr_manager.mk
endif

localclean:
	@echo "Cleaning project specific built files."
	$(foreach item,$(LEON_APP_OBJECTS_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_APP_URC_C_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_APP_URC_H_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_APP_OBJS),$(call SAFE_RM,$(item)))
	$(foreach item,$(PROJECTCLEAN),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_NN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_DYN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppObjBase)dynContextMaster.c $(DirAppObjBase)dynContextMaster.o,$(call SAFE_RM,$(item)))
	$(call SAFE_RM, $(foreach groupMK,$(MV_SHAVE_GROUPS),appsGrpDynContextRules$(groupMK).mk))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlibtail,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlibtemp,$(call SAFE_RM,$(item)))
ifeq ($(LEON_WITH_ARM_APP),yes)
	make -C armApp/ clean
endif
	$(foreach item,$(SHAVE_APP_LIBS),$(call SAFE_RM,$(item)))
	$(foreach item,$(SHAVE_NN_APP_LIBS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlibtail,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlibtemp,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/$(APPNAME).map,$(call SAFE_RM,$(item)))
	$(foreach item,$(CommonMlibFile).mlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(CommonMlibFile).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(ELF_FILE),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/$(APPNAME).mvcmd,$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/lst $(DirAppOutput)/$(APPNAME)_leon.sym,$(call SAFE_RM,$(item)))

# Clean split into different lines to avoid "execvp: /bin/bash: Argument list too long"
clean:
	@echo "Cleaning all built files from the MDK distribution and all project built files."
	$(foreach item,$(SHAVE_COMPONENT_OBJS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_SHARED_OBJECTS_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_SHARED_OBJECTS_REQUIRED_LRT),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_SHARED_OBJECTS_REQUIRED_LNN),$(call SAFE_RM,$(item)))
	$(foreach item,$(PROJECTCLEAN),$(call SAFE_RM,$(item)))
ifeq ($(MAKEDEPEND),yes)
	$(foreach item,$(LEON_APP_DEPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_DEPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(SHAVE_APP_DEPS),$(call SAFE_RM,$(item)))
endif
ifeq ($(LEON_WITH_ARM_APP),yes)
	make -C armApp/ clean
endif
	$(foreach item,$(LEON_APP_OBJECTS_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_APP_OBJS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_APP_URC_C_REQUIRED),$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_APP_URC_H_REQUIRED),$(call SAFE_RM,$(item)))
	$(call SAFE_RM, $(foreach groupMK,$(MV_SHAVE_GROUPS),appsGrpDynContextRules$(groupMK).mk))
	$(foreach item,$(LEON_RT_LIB_NAME).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlibtail,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_RT_LIB_NAME).rtlibtemp,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlibtail,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).rtlibtemp,$(call SAFE_RM,$(item)))
	$(foreach item,$(LEON_NN_LIB_NAME).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(SH_SWCOMMON_OBJS),$(call SAFE_RM,$(item)))
	$(foreach item,$(SH_SWCOMMON_GENASMS),$(call SAFE_RM,$(item)))
	$(foreach item,$(SHAVE_APP_LIBS),$(call SAFE_RM,$(item)))
	$(foreach item,$(SHAVE_NN_APP_LIBS),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_NN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(ALL_SHAVE_DYN_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppObjBase)dynContextMaster.c $(DirAppObjBase)dynContextMaster.o,$(call SAFE_RM,$(item)))
	$(call SAFE_RM,$(addsuffix .lrtgrpsyms,$(foreach group,$(MV_SHAVE_GROUPS),$($(group)))))
	$(call SAFE_RM,$(addsuffix .lrtappsyms,$(foreach group,$(MV_SHAVE_GROUPS),$(MV_$(group)_APPS))))
	$(foreach item,$(ALL_SHAVE_TEMP_APPS),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/$(APPNAME).map,$(call SAFE_RM,$(item)))
	$(foreach item,$(ELF_FILE),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/$(APPNAME).mvcmd,$(call SAFE_RM,$(item)))
	$(foreach item,$(CommonMlibFile).mlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(CommonMlibFile).mvlib,$(call SAFE_RM,$(item)))
	$(foreach item,$(ProjectShaveLib),$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/*,$(call SAFE_RM,$(item)))
	$(foreach item,$(DirAppOutput)/$(APPNAME)_leon.sym,$(call SAFE_RM,$(item)))


.INTERMEDIATE: $(SH_SWCOMMON_GENASMS) $(PROJECTINTERM)

# Target to print the content of a variable used for build system debugging
# To use this just issue a #> make print-<build_system_variable_name>
# ex: make print-MV_COMMON_BASE
#     MV_COMMON_BASE = ../../..
print-%  : ; @echo $* = $($*)

getToolsPath:
	@echo $(MV_TOOLS_BASE)/$(DETECTED_PLATFORM)
