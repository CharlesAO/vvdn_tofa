#functional_targets.mk - Features functional targets to be used with the new build flow
#
# Created on: Aug 19, 2013

# Default IP address for debugger connection, can be overridden
srvIP               ?= 127.0.0.1
srvPort             ?= 30001

#include rules for building the moviDebugConverter
include $(MV_COMMON_BASE)/debugger_build.mk

defaultDSCR =  $(DirDbgScrCommon)/default_debug_mdbg2.scr
defaultRSCR =  $(DirDbgScrCommon)/default_run_mdbg2.scr
defaultPSCR =  $(DirDbgScrCommon)/default_pipe_mdbg2.scr
defaultTPSCR = $(DirDbgScrCommon)/tcp_pipe_mdbg2.scr

MVDBG_SCRIPT_OPT  ?= -D:default_target=$(MV_DEFAULT_START_PROC_ID) $(MVDBGOPT) -srvIP:$(srvIP) -serverPort:$(srvPort) -D:elf=$(DirAppOutput)/$(APPNAME).elf

waves: $(DirAppOutput)/$(APPNAME).elf $(MVDBGCONV) $(MVDBGCONV_SOURCES) $(MVDBGCONV_HEADERS)
	@cp $(MV_COMMON_BASE)/scripts/debug/moviDebug/default_waves_tracer.scr $(debugSCR)
	$(MVDBG) $(MVDBG_SCRIPT_OPT) -D:run_opt=runw -D:save_opt="save $(TRACE_BUFFER_NAME) $(TRACE_BUFFER_SIZE) $(OUTPUT_TRACE_BUFFER_NAME)" -D:exit_opt=exit
	$(MVDBGCONV) $(OUTPUT_TRACE_BUFFER_NAME) -defhdr $(TRACE_SYSTEM_EVENTS_HDR) -evhdr $(LOCAL_TRACE_EVENTS_HDR) -vcd $(OUTPUT_VCD_FILE) -csv $(OUTPUT_CSV_FILE)

run: $(DirAppOutput)/$(APPNAME).elf
	@# This target is used to simply run your application.
	@# It performs load app; runw; exit
	@cp $(SourceDebugScript) $(debugSCR)
	@if [[ "$(SourceDebugScript)" == *"$(DirDbgScrCommon)"* ]]; \
	then \
		if [[ "$(ComponentList)" == *"PipePrint"* || "$(ComponentList_LOS)" == *"PipePrint"* || "$(ComponentList_LRT)" == *"PipePrint"* || "$(ComponentList_SVE)" == *"PipePrint"* ]]; \
		then \
			cat "$(defaultPSCR)" >> "$(debugSCR)"; \
		fi; \
		cat "$(defaultRSCR)" >> "$(debugSCR)"; \
	fi;
	cp $(debugSCR) $(debugSCR).tmp
	echo "source $(MV_COMMON_BASE)/scripts/debug/mdkTcl/mdkTclStart.tcl" > "$(debugSCR)"; \
	cat $(debugSCR).tmp  >> $(debugSCR)
	rm  $(debugSCR).tmp
	$(MVDBG) $(MVDBG_SCRIPT_OPT) --script $(debugSCR) -D:run_opt=runw -D:exit_opt=exit  $(debugOptions)

ifneq "$(MVDBG_DISABLE_INTERACTIVE)" "yes"
MVDBG_RUN_INTERACTIVE=--interactive
endif

MVDBG_RUN_OPT?=run
debug: $(DirAppOutput)/$(APPNAME).elf
	@# This target is used to load and run your application, but interactive debug is still possible during exection
	@# It performs load app; run;
	@cp $(SourceDebugScript) $(debugSCR)
	$(ECHO) if [[ "$(SourceDebugScript)" == *"$(DirDbgScrCommon)"* ]]; \
	then \
		if [[ "$(ComponentList)" == *"PipePrint"* || "$(ComponentList_LOS)" == *"PipePrint"* || "$(ComponentList_LRT)" == *"PipePrint"* || "$(ComponentList_SVE)" == *"PipePrint"* ]]; \
		then \
			cat "$(defaultPSCR)" >> "$(debugSCR)"; \
		fi; \
		cat "$(defaultDSCR)" >> "$(debugSCR)"; \
	fi;
	cp $(debugSCR) $(debugSCR).tmp
	echo "source $(MV_COMMON_BASE)/scripts/debug/mdkTcl/mdkTclStart.tcl" > "$(debugSCR)"; \
	cat $(debugSCR).tmp  >> $(debugSCR)
	rm  $(debugSCR).tmp
	if [ "$(ECHO)" == "" ]; then echo '$(MVDBG) $(MVDBG_SCRIPT_OPT) $(MVDBG_RUN_INTERACTIVE) --script $(debugSCR) -D:run_opt=$(MVDBG_RUN_OPT) $(debugOptions)'; fi;
	$(MVDBG) $(MVDBG_SCRIPT_OPT) $(MVDBG_RUN_INTERACTIVE) --script $(debugSCR) -D:run_opt=$(MVDBG_RUN_OPT) $(debugOptions)

debug_telnet: defaultPSCR=$(defaultTPSCR)
debug_telnet: debug

run_telnet: defaultPSCR=$(defaultTPSCR)
run_telnet: run

load: $(DirAppOutput)/$(APPNAME).elf
	@# This target is used to simply start debugger and load your application ready for interactive debug
	@# It performs load app;
	@cp $(SourceDebugScript) $(debugSCR)
		$(ECHO) if [[ "$(SourceDebugScript)" == *"$(DirDbgScrCommon)"* ]]; \
	then \
		if [[ "$(ComponentList)" == *"PipePrint"* || "$(ComponentList_LOS)" == *"PipePrint"* || "$(ComponentList_LRT)" == *"PipePrint"* || "$(ComponentList_SVE)" == *"PipePrint"* ]]; \
		then \
			cat "$(defaultPSCR)" >> "$(debugSCR)"; \
		fi; \
		cat "$(defaultDSCR)" >> "$(debugSCR)"; \
	fi;
	cp $(debugSCR) $(debugSCR).tmp
	echo "source $(MV_COMMON_BASE)/scripts/debug/mdkTcl/mdkTclStart.tcl" > "$(debugSCR)"; \
	cat $(debugSCR).tmp  >> $(debugSCR)
	rm  $(debugSCR).tmp
	$(MVDBG) $(MVDBG_SCRIPT_OPT) --script $(debugSCR) --interactive -D:run_opt=halt -D:exit_opt=" "

debugi:
	@# Simply launch moviDebug interactively
	@mkdir -p $(dir $(debugSCR))
	@cp $(SourceDebugScript) $(debugSCR)
	$(ECHO) if [[ "$(SourceDebugScript)" == *"$(DirDbgScrCommon)"* ]]; \
	then \
		if [[ "$(ComponentList)" == *"PipePrint"* || "$(ComponentList_LOS)" == *"PipePrint"* || "$(ComponentList_LRT)" == *"PipePrint"* || "$(ComponentList_SVE)" == *"PipePrint"* ]]; \
		then \
			cat "$(defaultPSCR)" >> "$(debugSCR)"; \
		fi; \
		cat "$(defaultDSCR)" >> "$(debugSCR)"; \
	fi;
	cp $(debugSCR) $(debugSCR).tmp
	cat $(debugSCR).tmp  >> $(debugSCR)
	rm  $(debugSCR).tmp
	$(MVDBG) $(MVDBG_SCRIPT_OPT) --script $(MV_COMMON_BASE)/scripts/debug/mdkTcl/mdkTclStart.tcl -D:run_opt=$(MVDBG_RUN_OPT) --interactive


##############################################################
##############################################################

start_server:
	$(ECHO) while true; \
		do $(MVSVR) $(MV_SRV_EXTRA_OPT) -tcpPort:$(srvPort); \
		[[ $$? == 0 ]] && break; \
	done

start_simulator:
	$(ECHO) while true; do $(MVSIM) $(MVSIMOPT) -tcpip:$(srvPort) -q; done

start_simulator_full:
	$(ECHO) while true; do $(MVSIM) $(MVSIMOPT) -ca -tcpip:$(srvPort) -darw -v:4; done

#
# This target will run moviSim in a standalone instance, then launch the
# program in it via the debugger. A couple of SHELL scripts are used in order
# to get the next free port, then release it, in a computer-wide manner if run
# under Jenkins. If run on a local computer, that'll also work, provided other
# users on the computer won't launch the same 'make sim'
#
MOVISIM_INIT_RETRY_COUNT ?= 3
sim: $(DirAppOutput)/$(APPNAME).elf
	[[ ! -d $(DirOutputLst) ]] && mkdir -p $(DirOutputLst) || true
	( \
		MOVISIM_PORT=$$($(MV_COMMON_BASE)/scripts/movisim/get_movisim_port.sh) ; \
		echo "Got free port $$MOVISIM_PORT" ; \
		MOVISIM_LAUNCH_RETRY_COUNT=0 ; \
		until [ $$MOVISIM_LAUNCH_RETRY_COUNT -ge $(MOVISIM_INIT_RETRY_COUNT) ] ; \
		do \
			$(MVSIM) $(MVSIMOPT) $(otherMovisimOptions) $(debugOptions) -ca -tcpip:$$MOVISIM_PORT -darw -v:4 > $(DirOutputLst)/leonos_movisim.lst & \
			MOVISIM_PID=$! ; \
			jobs %% && break ; \
			MOVISIM_LAUNCH_RETRY_COUNT=$$[$$MOVISIM_LAUNCH_RETRY_COUNT+1] ; \
		done ; \
		while [[ -z "$$(ss -ln | grep $$MOVISIM_PORT)" ]] ; do sleep 0.1 ; done ; \
		$(MAKE) debug $(filter-out sim,$(MAKECMDGOALS)) MVDBG_DISABLE_INTERACTIVE=yes srvPort=$$MOVISIM_PORT MVDBG_RUN_OPT=runw; \
		$(MV_COMMON_BASE)/scripts/movisim/release_movisim_port.sh $$MOVISIM_PORT \
	)
	$(ECHO)cat $(DirOutputLst)/leonos_movisim.lst  | grep "PC=0x" | sed 's/^PC=0x//g' > $(DirOutputLst)/leonos_movisim.dasm || true
	$(ECHO)cat $(DirOutputLst)/leonos_movisim.lst  | grep -w "UART:\|DEBUG:" > $(DirOutputLst)/leonos_movisim_uart.txt || true

# This target will use movisim to load elf and run in a standalone instance without debugger
sim_standalone:
	@mkdir -p $(DirOutputLst)
	$(MVSIM) $(MVSIMOPT) $(SIM_OPT) $(otherMovisimOptions) $(debugOptions) \
	-l:$(MV_DEFAULT_START_PROC_ID):$(DirAppOutput)/$(APPNAME).elf $(MVSIMOUTOPT) -q -nodasm -darw | tee $(DirOutputLst)/leonos_movisim.lst


# Programs the target application into SPI Memory on MV0182/MV0235
flash: MVCMDSIZE = $(shell du -b $(MvCmdfile))
flash: $(MvCmdfile)
	$(ECHO)cat $(DefaultFlashScript) | \
		sed 's!XX_FLASHER_ELF_XX!$(MV_COMMON_BASE)/utils/jtag_flasher/flasher_$(MV_SOC_REV).elf!' | \
		sed 's!XX_TARGET_MVCMD_SIZE_XX!$(word 1, $(MVCMDSIZE))!' | \
		sed 's!XX_TARGET_MVCMD_XX!$(MvCmdfile)!' > $(flashSCR)
	$(ECHO)$(MVDBG) $(FLASH_SCRIPT_OPT) $(MVDBGOPT) -srvIP:$(srvIP) -serverPort:$(srvPort)
	$(ECHO) if diff $(MvCmdfile) $(MvCmdfile).readback; then \
		echo "Reading back from DDR unchanged [OK]"; \
		rm $(MvCmdfile).readback; \
	else \
		echo "Reading back from DDR was different! [BAD]"; \
		exit 1; \
	fi

flash_eMMC: MVCMDSIZE = $(shell du -b $(MvCmdfile))
flash_eMMC: $(MvCmdfile)
	$(ECHO)cat $(DefaultEmmcFlashScript) | \
		sed 's!XX_FLASHER_ELF_XX!$(MV_COMMON_BASE)/utils/jtag_flasher/mmc_flasher_$(MV_SOC_REV).elf!' | \
		sed 's!XX_TARGET_MVCMD_SIZE_XX!$(word 1, $(MVCMDSIZE))!' | \
		sed 's!XX_TARGET_MVCMD_XX!$(MvCmdfile)!' > $(flashSCR)
	$(ECHO)$(MVDBG) $(FLASH_SCRIPT_OPT) $(MVDBGOPT) -srvIP:$(srvIP) -serverPort:$(srvPort)
	$(ECHO) if diff $(MvCmdfile) $(MvCmdfile).readback; then \
		echo "Reading back from DDR unchanged [OK]"; \
		rm $(MvCmdfile).readback; \
	else \
		echo "Reading back from DDR was different! [BAD]"; \
		exit 1; \
	fi

flash_erase:
	$(ECHO)cat $(DefaultEraseScript) | \
		sed 's!XX_FLASHER_ELF_XX!$(MV_COMMON_BASE)/utils/jtag_flasher/flasher_$(MV_SOC_REV).elf!' | \
		sed 's!XX_TARGET_MVCMD_SIZE_XX!0!' > $(eraseSCR)
	$(ECHO)$(MVDBG) $(FLASH_SCRIPT_ERASE_OPT) $(MVDBGOPT) -srvIP:$(srvIP) -serverPort:$(srvPort)

flash_erase_eMMC:
	$(ECHO)cat $(DefaultEmmcEraseScript) | \
		sed 's!XX_FLASHER_ELF_XX!$(MV_COMMON_BASE)/utils/jtag_flasher/mmc_flasher_$(MV_SOC_REV).elf!' | \
		sed 's!XX_TARGET_MVCMD_SIZE_XX!0!' > $(eraseSCR)
	$(ECHO)$(MVDBG) $(FLASH_SCRIPT_ERASE_OPT) $(MVDBGOPT) -srvIP:$(srvIP) -serverPort:$(srvPort)

$(DirOutputReport)/memviz.html : $(DirAppOutput)/$(APPNAME).map
	@echo "Generating memory view..."
	$(ECHO) mkdir -p $(DirOutputReport)
	$(ECHO) env python2 $(MV_COMMON_BASE)/utils/memviz.py \
                            -i $(DirAppOutput)/$(APPNAME).map \
                            -p $(MV_SOC_PLATFORM)             \
                            -a $(MV_SOC_REV)              \
	                    -o $(DirOutputReport)/memviz.html \
                            -c $(MV_COMMON_BASE)/utils/memviz_style.css

report: $(DirOutputReport)/memviz.html

report-open: report
	@echo $(shell xdg-open "$(DirOutputReport)/memviz.html" )

HELP_SETUP_TXT?=""

help:
	@echo -e -n $(HELP_SETUP_TXT)
	@echo "'make help'                      : Shows this message"
	@echo "'make all'                       : Builds the application and generates the"
	@echo "                                   .sym and .mvcmd files and the lst folder"
	@echo "'make clean'                     : Cleans all built files from all over the"
	@echo "                                   MDK distribution (including build drivers "
	@echo "                                   files)"
	@echo "'make run'                       : Builds the application's .elf file"
	@echo "                                   and runs it on a target via moviDebug (non-interactive)"
	@echo "                                   Application is loaded and execution"
	@echo "                                   is started automatically"
	@echo "'make debug'                     : Builds the application and interactively debugs it"
	@echo "                                   on a target via moviDebug. Application is loaded"
	@echo "                                   and execution is started automatically"
	@echo "'make list'                      : Lists all the possible targets for the current Makefile"
	@echo "'make lst'                       : Builds the symbol listings using moviDump"
	@echo "'make load'                      : Builds application and load target elf into debugger"
	@echo "                                   but don't start execution. (allows setting "
	@echo "                                   breakpoints in advance)"
	@echo "'make debugi'                    : Simply launches the debugger for interactive session"
	@echo "                                   (no commands executed)"
	@echo "'make start_server'              : Starts moviDebug server"
	@echo "'make start_simulator'           : Starts moviSim in quiet mode"
	@echo "'make start_simulator_full       : Starts moviSim in verbose full mode"
	@echo "'make report'                    : Generates a visual report of memory utilization."
	@echo "                                   Needs application to be build previously"
	@echo "'make show_tools'                : Displays tool paths"
	@echo "'make flash'                     : Programs SPI flash of MV0182/MV0235 with"
	@echo "                                   your current mvcmd"
	@echo "'make flash_erase'               : Erases SPI flash of MV0182"
	@echo "'make flash_eMMC'                : Programs MMC of MV0235 with your current mvcmd"
	@echo "'make flash_erase_eMMC'          : Erase MMC device of MV0235"
	@echo "'make secure'                    : Builds MVCMD for Secure Boot"
	@echo "'make gen_keys'                  : Generates key set for Secure Boot"
	@echo
	@echo "Optional Parameters:"
	@echo "srvIP=192.168.1.100              : Connects to moviDebugserver on a different PC"
	@echo "VERBOSE=yes                      : Echoes full build commands during make"
	@echo "DISABLE_PRINTF=yes               : Turns off printf, useful when generating Boot"
	@echo "                                   image"
	@echo "MV_BUILD_CONFIG=debug/release    : Configures if build is for debug or release"
	@echo "                                   Makes sure you clean between changing configs"
	@echo "MvCmdfile=path/to/file.mvcmd     : given to \"make flash\" to write a "
	@echo "                                   different .mvcmd file"
	@echo

show_tools:
	@echo "SHAVE Toolchain..."
	@echo "   MVASM   :  $(MVASM)"
	@echo "   MVDUMP  :  $(MVDUMP)"
	@echo "   MVSIM   :  $(MVSIM)"
	@echo "   MVDBG   :  $(MVDBG)"
	@echo "   MVDBG2  :  $(MVDBG2)"
	@echo "   MVDBGTCL:  $(MVDBGTCL)"
	@echo "   MVCONV  :  $(MVCONV)"
	@echo "   MVCC    :  $(MVCC)"
	@echo "   MVSVR   :  $(MVSVR)"
	@echo ""
	@echo "Sparc (Leon) or ARMv8 Toolchain..."
	@echo "   CC      :  $(CC)"
	@echo "   LD      :  $(LD)"
	@echo "   OBJCOPY :  $(OBJCOPY)"
	@echo "   AS      :  $(AS)"
	@echo "   AR      :  $(AR)"
	@echo "   STRIP   :  $(STRIP)"
	@echo "   OBJDUMP :  $(OBJDUMP)"
	@echo "   READELF :  $(READELF)"
	@echo "   CPP_FILT:  $(CPP_FILT)"

.PHONY: list
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' || true

ifeq ($(UNAME),GNU/Linux)
  DEVNULL=/dev/null
else
  DEVNULL=nul
endif

$(DirAppObjBase)%_urc.c : %.urc Makefile
	@echo "USBRC : Generating $@ from $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(USBRC) $(USBRCOPT) -Io - -o - $< > $@

$(DirAppObjBase)%.h : %.urc Makefile
	@echo "USBRC : Generating $@ from $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(USBRC) $(USBRCOPT) -o $(DEVNULL) -Ho $@ $(filter-out "$(DirAppObjBase)",$(subst _urc.cusbrcgen,.urc,$<))

#USBRC generated object files have slightly different rules because they have to
#not generate dependency files one on another, they themselves being generated files
#in their turn. So we take all other rules but we exclude the -MD option from this one
$(DirAppObjBase)%_urc.o : $(DirAppObjBase)%_urc.c $(DirAppObjBase)%.h $(LEON_HEADERS) Makefile
	$(call PRINT_USING_CCACHE)
	@echo "Leon USBRC CC   : $<"
	@test -d $(@D) || mkdir -p $(@D)
	$(ECHO) $(CC) -c $(CONLY_OPT) $(filter-out -MD -MP,$(CCOPT)) $(call RESOLVE_TARGET_PATH,$<) -o $(call RESOLVE_TARGET_PATH,$@)

# Targets for generating MVCMDs for use with the MA2155 Secure Boot feature

# Directory containing the keys.
# This can be overriden if keys are to be kept in a central area.
SECUREBOOT_KEY_PATH ?= $(DirAppRoot)/keys
# Base filename for key files.
# Defaults to application name, but can be overriden if keys are shared between apps.
SECUREBOOT_KEY_NAME ?= $(APPNAME)
# Generates full key paths
SECUREBOOT_PUBLIC_KEY ?= $(SECUREBOOT_KEY_PATH)/$(SECUREBOOT_KEY_NAME).pubkey
SECUREBOOT_PRIVATE_KEY ?= $(SECUREBOOT_KEY_PATH)/$(SECUREBOOT_KEY_NAME).privkey
SECUREBOOT_AES_KEY ?= $(SECUREBOOT_KEY_PATH)/$(SECUREBOOT_KEY_NAME).aeskey

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
GENKEYSOPT = -cv:$(MV_SOC_REV) -secureAesKey:"$(SECUREBOOT_AES_KEY)"
else
GENKEYSOPT = -cv:$(MV_SOC_REV) -securePrivateKey:"$(SECUREBOOT_PRIVATE_KEY)" -securePublicKey:"$(SECUREBOOT_PUBLIC_KEY)" -secureAesKey:"$(SECUREBOOT_AES_KEY)"
endif

# Generate a set of keys:
# Public key  - to be programmed in to EFuses
# Private key - used for generating secure MVCMDs. Must be kept secure.
# AES key     - used for generating secure MVCMDs and to be programmed in to EFuses
# This target will not overwrite existing key files.


.PHONY: gen_keys
gen_keys:
	$(ECHO) if [ -e "$(SECUREBOOT_PUBLIC_KEY)" -o -e "$(SECUREBOOT_PRIVATE_KEY)" -o -e "$(SECUREBOOT_AES_KEY)" ]; then \
	echo "Not overwriting existing keys"; \
	else \
	mkdir -p $(SECUREBOOT_KEY_PATH); \
	$(MVCONV) -generateKeys $(GENKEYSOPT); \
	echo "Generated keys:" ; \
	if [ "$(MV_SOC_REV)" == ma2480 ] || [ "$(MV_SOC_REV)" == ma2485 ]; then \
	echo "  AES key    : $(SECUREBOOT_AES_KEY)" ; \
	else \
	echo "  Public key : $(SECUREBOOT_PUBLIC_KEY)" ; \
	echo "  Private key: $(SECUREBOOT_PRIVATE_KEY)" ; \
	echo "  AES key    : $(SECUREBOOT_AES_KEY)" ; \
	echo "  Private key must not be released" ; \
	fi; \
	fi


# Suffix for a secure application MVCMD
MvCmdfileSecureSuffix ?= _secure

# The MVCMD filename for the secure application.
MvCmdfileSecure    	 ?= $(DirAppOutput)/$(APPNAME)$(MvCmdfileSecureSuffix).mvcmd

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480 ma2485))
  MVCMDOPT_SECURE = -secureBoot -secureAesKey:"$(SECUREBOOT_AES_KEY)"
else
  MVCMDOPT_SECURE = -secureBoot -securePrivateKey:"$(SECUREBOOT_PRIVATE_KEY)" -secureAesKey:"$(SECUREBOOT_AES_KEY)"
endif


# General rule to make a secure MVcmd from a elf
# Requires keys to be already generated.
$(DirAppOutput)/%$(MvCmdfileSecureSuffix).mvcmd : $(DirAppOutput)/%.elf
	@echo "Generating Secure MVCMD boot image: $(call RESOLVE_TARGET_PATH,$@)"
	@echo " From input elf file              : $(call RESOLVE_TARGET_PATH,$(^))"
	if [ "$(MV_SOC_REV)" == ma2480 ] || [ "$(MV_SOC_REV)" == ma2485 ]; then \
	echo " Using AES Key                    : $(call RESOLVE_TARGET_PATH,$(SECUREBOOT_AES_KEY))"
	else \
	echo " Using AES Key                    : $(call RESOLVE_TARGET_PATH,$(SECUREBOOT_AES_KEY))"
	echo " And ECC Private Key              : $(call RESOLVE_TARGET_PATH,$(SECUREBOOT_PRIVATE_KEY))"
	fi
	$(ECHO) mkdir -p $(dir $(call RESOLVE_TARGET_PATH,$@))
	$(ECHO) $(MVCONV) $(MVCONVOPT) $(MVCMDOPT) $(MVCMDOPT_SECURE) $(^) -mvcmd:$(@)  $(DUMP_NULL)

# Top level target for building a secure application MVCMD
.PHONY: secure
secure: $(MvCmdfileSecure)
