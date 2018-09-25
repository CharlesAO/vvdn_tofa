#-------[ Local shave applications build rules ]-----#
# Describe the rules for building MesgOneApp and SGBMApp applications. Simple rule
# specifying which objects build up the said application.
# Each application will be built into a library.
ENTRYPOINTS_sgbm= -e $(SGBMApp_MainEntry) -u __init_array_start -u __fini_array_start -u __init_array_end -u __fini_array_end --gc-sections
$(SGBMApp).mvlib : $(SHAVE_SGBMApp_OBJS)
	$(ECHO) $(LD) $(ENTRYPOINTS_sgbm) $(MVLIBOPT) $(SHAVE_SGBMApp_OBJS) -o $@

ENTRYPOINTS_cost = -e $(costApp_MainEntry) -u __init_array_start -u __fini_array_start -u __init_array_end -u __fini_array_end --gc-sections
$(costApp).mvlib : $(SHAVE_costApp_OBJS)
	$(ECHO) $(LD) $(ENTRYPOINTS_cost) $(MVLIBOPT) $(SHAVE_costApp_OBJS) -o $@
