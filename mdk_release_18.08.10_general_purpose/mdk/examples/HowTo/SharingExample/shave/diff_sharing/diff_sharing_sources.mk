#subinclude showing which files 

#Choosing C sources the SharingExample application on shave
diff_sharing = shave/diff_sharing

SHAVE_C_SOURCES_diff_sharing = $(wildcard $(DirAppRoot)/shave/diff_sharing/*.c)
#Choosing ASM sources for the shave SharingExample app on shave
SHAVE_ASM_SOURCES_diff_sharing = $(wildcard $(DirAppRoot)/shave/diff_sharing/myriad2Asm/*.asm)

#Generating list of required generated assembly files for the SharingExample app on shave
SHAVE_GENASMS_diff_sharing = $(patsubst %.c,$(DirAppObjBase)%.asmgen,$(SHAVE_C_SOURCES_diff_sharing))
#Generating required objects list from sources
SHAVE_diff_sharing_OBJS = $(patsubst %.asm,$(DirAppObjBase)%_shave.o,$(SHAVE_ASM_SOURCES_diff_sharing)) \
                   $(patsubst $(DirAppObjBase)%.asmgen,$(DirAppObjBase)%_shave.o,$(SHAVE_GENASMS_diff_sharing))

#update clean rules with our generated files
PROJECTCLEAN += $(SHAVE_GENASMS_diff_sharing) $(SHAVE_diff_sharing_OBJS)
#Uncomment below to reject generated shave as intermediary files (consider them secondary)
PROJECTINTERM += $(SHAVE_GENASMS_diff_sharing)
