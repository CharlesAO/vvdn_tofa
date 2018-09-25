#adding support for individual eanblement of debug for file compilation

ISOLATED_OPTIONS_LEON_OS_C_FILES   :=
ISOLATED_OPTIONS_LEON_OS_CPP_FILES :=
ISOLATED_OPTIONS_LEON_RT_C_FILES   :=
ISOLATED_OPTIONS_LEON_RT_CPP_FILES :=
ISOLATED_OPTIONS_SHAVE_C_FILES             :=
ISOLATED_OPTIONS_SHAVE_CPP_FILES           :=

-include projectDebugFiles.mk

ifneq ($(ISOLATED_OPTIONS_LEON_OS_C_FILES),)
define cLeonIsolatedCFilesRules_template

$(2)_CCOPT += 

$$(DirAppObjBase)$$(DirAppRoot)/$(1).o : $(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Leon CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CC) -c $$(CONLY_OPT) $$($(2)_PLUS_CCOPT) $$(filter-out $$($(2)_MINUS_CCOPT),$$(CCOPT)) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)
	
endef
endif

$(foreach isolatedCOSFile,$(ISOLATED_OPTIONS_LEON_OS_C_FILES),$(eval $(call cLeonIsolatedCFilesRules_template,$(isolatedCOSFile),$(subst /,_,$(subst .,_,$(isolatedCOSFile))))))

ifneq ($(ISOLATED_OPTIONS_LEON_OS_CPP_FILES),)
define cLeonIsolatedCFilesRules_template

$(2)_CPPOPT += 

$$(DirAppObjBase)$$(DirAppRoot)/$(1).o : $(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Leon CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CXX) -c $$(CPPONLY_OPT) $$($(2)_PLUS_CPPOPT) $$(filter-out -Werror-implicit-function-declaration,$$(CCOPT)) $$(filter-out $$($(2)_MINUS_CPPOPT),$$(CPPOPT)) -fno-rtti -fno-exceptions  $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)
	
endef
endif

$(foreach isolatedCPPOSFile,$(ISOLATED_OPTIONS_LEON_OS_CPP_FILES),$(eval $(call cLeonIsolatedCFilesRules_template,$(isolatedCPPOSFile),$(subst /,_,$(subst .,_,$(isolatedCPPOSFile))))))

ifneq ($(ISOLATED_OPTIONS_LEON_RT_C_FILES),)
define cLeonIsolatedRTCFilesRules_template

$(2)_CCOPT_RT += 

$$(DirAppObjBase)$$(DirAppRoot)/$(1)_lrt.o : $(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Leon CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CC) -c $$(CONLY_OPT) $$($(2)_PLUS_CCOPT_RT) $$(filter-out $$($(2)_MINUS_CCOPT_RT),$$(CCOPT_LRT)) -O0 $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)
	
endef
endif

$(foreach isolatedCOSFile,$(ISOLATED_OPTIONS_LEON_RT_C_FILES),$(eval $(call cLeonIsolatedRTCFilesRules_template,$(isolatedCOSFile),$(subst /,_,$(subst .,_,$(isolatedCOSFile))))))

ifneq ($(ISOLATED_OPTIONS_LEON_RT_CPP_FILES),)
define cLeonIsolatedRTCPPFilesRules_template

$(2)_CPPOPT_RT += 

$$(DirAppObjBase)$$(DirAppRoot)/$(1)_lrt.o : $(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Leon CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CXX) -c $$(CPPONLY_OPT) $$($(2)_PLUS_CPPOPT_RT) $$(filter-out -Werror-implicit-function-declaration,$$(CCOPT_LRT)) $$(filter-out $$($(2)_MINUS_CPPOPT_RT),$$(CPPOPT)) -fno-rtti -fno-exceptions  $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)
	
endef
endif

$(foreach isolatedCPPOSFile,$(ISOLATED_OPTIONS_LEON_RT_CPP_FILES),$(eval $(call cLeonIsolatedRTCPPFilesRules_template,$(isolatedCPPOSFile),$(subst /,_,$(subst .,_,$(isolatedCPPOSFile))))))

ifneq ($(ISOLATED_SHAVE_C_FILES),)
define cShaveIsolatedCFilesRules_template

$(2)_MVCCOPT +=

 ifeq ($$(MVCC_SKIP_ASM),yes)
$$(DirAppObjBase)$$(DirAppRoot)/$(1)_shave.o : $(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Shave CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(MVCC) $$(filter-out $$($(2)_MINUS_MVCCOPT),$$(MVCCOPT)) $$($(2)_PLUS_MVCCOPT) $$(MVCCASMOPT) $$(MVCC_DEBUGFLAGS) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@) $$(DUMP_NULL)
	
else
$$(DirAppObjBase)$$(DirAppRoot)/$(1).asmgen : $(1).c $$(SHAVE_C_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Shave CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(MVCC) $$(filter-out $$($(2)_MINUS_MVCCOPT),$$(MVCCOPT)) $$($(2)_PLUS_MVCCOPT) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@) $$(DUMP_NULL)
	
endif
	
endef
endif

$(foreach isolatedCSHVFile,$(ISOLATED_OPTIONS_SHAVE_C_FILES),$(eval $(call cShaveIsolatedCFilesRules_template,$(isolatedCSHVFile),$(subst /,_,$(subst .,_,$(isolatedCSHVFile))))))

ifneq ($(ISOLATED_SHAVE_CPP_FILES),)
define cShaveIsolatedCPPFilesRules_template

$(2)_MVCCPPOPT +=

ifeq ($$(MVCC_SKIP_ASM),yes)
$$(DirAppObjBase)$$(DirAppRoot)/$(1)_shave.o : $(1).cpp $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Shave CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(MVCC) $$(filter-out $$($(2)_MINUS_MVCCOPT),$$(MVCCOPT)) $$($(2)_PLUS_MVCCPPOPT) $$(MVCCASMOPT) $$(MVCC_DEBUGFLAGS) $$(filter-out $$($(2)_MINUS_MVCCPPOPT,$$(MVCCPPOPT)) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)  $$(DUMP_NULL)
	
else
$$(DirAppObjBase)$$(DirAppRoot)/$(1).asmgen : $(1).cpp $$(SHAVE_C_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Shave CC debug : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(MVCC) $$(filter-out $$($(2)_MINUS_MVCCOPT),$$(MVCCOPT)) $$($(2)_PLUS_MVCCPPOPT) $$(MVCCASMOPT) $$(MVCC_DEBUGFLAGS) $$(filter-out $$($(2)_MINUS_MVCCPPOPT,$$(MVCCPPOPT)) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)  $$(DUMP_NULL)
	
endif
	
endef
endif

$(foreach isolatedCPPSHVFile,$(ISOLATED_OPTIONS_SHAVE_CPP_FILES),$(eval $(call cShaveIsolatedCPPFilesRules_template,$(isolatedCPPSHVFile),$(subst /,_,$(subst .,_,$(isolatedCPPSHVFile))))))


