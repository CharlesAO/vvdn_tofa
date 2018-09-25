ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2480 ma2485))
    LOCKABLE_FILES  = drivers/myriad2/socDrivers/leon/bm/arch/ma2x8x/src/DrvCpr
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma2x8x/src/DrvCprHgl
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma2x8x/src/DrvDdr
else
    LOCKABLE_FILES  = drivers/myriad2/socDrivers/leon/bm/src/DrvCpr
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/hgl/arch/ma2x5x/src/HglCpr
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/hgl/arch/ma2x5x/src/HglCprMa2x5x
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/src/DrvDdr
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma2x5x/src/DrvDdrArchMa2x5x
    LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma2x5x/src/DrvDdrMa2x5x
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2150 ma2155))
        LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma215x/src/DrvDdrArchMa215x
    else
        LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/arch/ma245x/src/DrvDdrArchMa245x
    endif
    ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV), ma2450))
        LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/src/DrvEfuse
        LOCKABLE_FILES += drivers/myriad2/socDrivers/leon/bm/src/DrvEfuseIO
    endif
endif

LOCKABLE_FILES += swCommon/myriad2/src/dbgTracer

define PowerObjects_template

$$(DirAppObjBase)$$(MV_COMMON_BASE)/$(1).lo : $$(MV_COMMON_BASE)/$(1).c $$(LEON_HEADERS) Makefile
	$$(call PRINT_USING_CCACHE)
	@echo "Leon CC : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(CC) -c $$(CONLY_OPT) $$(CCOPT) $$(call RESOLVE_TARGET_PATH,$$<) -o $$(call RESOLVE_TARGET_PATH,$$@)

$$(DirAppObjBase)$$(MV_COMMON_BASE)/$(1).o : $$(DirAppObjBase)$$(MV_COMMON_BASE)/$(1).lo Makefile
	@echo "Gathering LOS wake-up code : $$<"
	@test -d $$(@D) || mkdir -p $$(@D)
	$$(ECHO) $$(OBJCOPY) --prefix-alloc-sections=.lockable $$(call RESOLVE_TARGET_PATH,$$<) $$(call RESOLVE_TARGET_PATH,$$@)

endef

$(foreach objfile,$(LOCKABLE_FILES),$(eval $(call PowerObjects_template,$(objfile))))
