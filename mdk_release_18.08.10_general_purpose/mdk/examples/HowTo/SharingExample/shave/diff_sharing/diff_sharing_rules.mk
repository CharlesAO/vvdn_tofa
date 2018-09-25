#subinclude showing how to build this shave application

#Describe the rule for building the diff_sharing application. Simple rule specifying
#which objects build up the said application. The application will be built into a library
$(diff_sharing).mvlib : $(SHAVE_diff_sharing_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(MVLIBOPT) $(SHAVE_diff_sharing_OBJS) $(PROJECT_SHAVE_LIBS) $(CompilerANSILibs) -o $@
