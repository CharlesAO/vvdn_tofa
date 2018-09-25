#mbinflow.mk - simple mbinflow.mk rules collection to be used with the Movidius build flow
#
# Created on: Aug 27, 2013

$(DirAppObjBase)%.mobj : %.asmgen $(SHAVE_ASM_HEADERS) Makefile
	@echo Shave assembling $@
	$(ECHO) mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT) $< -o:$@

$(DirAppObjBase)%.mobj : %.asm $(SHAVE_ASM_HEADERS) Makefile
	@echo Shave assembling $@
	$(ECHO) mkdir -p $(@D)
	$(ECHO) $(MVASM) $(MVASMOPT) $< -o:$@

$(DirAppObjBase)%_raw.o : %.mbin
	$(ECHO) mkdir -p $(@D)
	$(ECHO) $(OBJCOPY)  -I binary --reverse-bytes=4 --rename-section .data=.ddr_direct.data \
	--redefine-sym  _binary_$(subst -,_,$(subst /,_,$(subst .,_,$<)))_start=$(lastword $(filter-out mbin,$(subst /, ,$(subst ., ,$<))))Mbin \
	-O elf32-sparc -B sparc $< $@
