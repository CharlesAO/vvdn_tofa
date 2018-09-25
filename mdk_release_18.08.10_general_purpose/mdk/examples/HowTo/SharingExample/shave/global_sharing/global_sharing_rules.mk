#subinclude showing how to build this shave application

#symbols uniqufy file (relative path to root of the project):
SYMBOLS_UNIQ_SHAVE2= ./shave/global_sharing/global_sharing2.symuniq
SYMBOLS_UNIQ_SHAVE3= ./shave/global_sharing/global_sharing3.symuniq
#The symuniq file has to only contain a list of pairs of "old new" symbols
#We need as many symuniq files as instances of the application we plan to use.
#in this case they are two: one on shave2 and the other one on shave3

#Describe the rule for building the global_sharing application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
$(global_sharing).mvlib : $(SHAVE_global_sharing_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(SHAVE_global_sharing_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@

#and describe the unique libraries rules. For this case only the shared1 and shared2 symbols need
#uniquifying
%.shv2uniqlib : %.shv2lib
	$(ECHO) $(OBJCOPY) --redefine-syms=$(SYMBOLS_UNIQ_SHAVE2) $< $@
#and describe the unique libraries rules. For this case only the shared1 and shared2 symbols need
#uniquifying
%.shv3uniqlib : %.shv3lib
	$(ECHO) $(OBJCOPY) --redefine-syms=$(SYMBOLS_UNIQ_SHAVE3) $< $@
