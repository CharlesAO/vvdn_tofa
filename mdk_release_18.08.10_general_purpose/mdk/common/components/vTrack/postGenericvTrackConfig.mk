ENTRYPOINTS_fm = -e $(fm_MainEntry) --gc-sections
$(fmApp).mvlib : $(SHAVE_fm_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS_fm) $(MVLIBOPT) $(SHAVE_fm_OBJS) -o $@

ENTRYPOINTS_of = -e $(of_MainEntry) --gc-sections
$(ofApp).mvlib : $(SHAVE_of_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS_of) $(MVLIBOPT) $(SHAVE_of_OBJS) -o $@

ENTRYPOINTS_ppm = -e $(ppm_MainEntry) -u fifoCommMasterRun --gc-sections
$(ppmApp).mvlib : $(SHAVE_ppm_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS_ppm) $(MVLIBOPT) $(SHAVE_ppm_OBJS) $(filter-out $(CommonMlibFile).mvlib,$(PROJECT_SHAVE_LIBS)) -o $@

ENTRYPOINTS_pp_corners = -e $(pp_corners_MainEntry) -u cornerFilter --gc-sections
$(pp_cornersApp).mvlib : $(SHAVE_pp_corners_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS_pp_corners) $(MVLIBOPT) $(SHAVE_pp_corners_OBJS) $(filter-out $(CommonMlibFile).mvlib,$(PROJECT_SHAVE_LIBS)) -o $@

ENTRYPOINTS_pp_gauss = -e $(pp_gauss_MainEntry) -u PyrLevel -u gaussInit --gc-sections
$(pp_gaussApp).mvlib : $(SHAVE_pp_gauss_OBJS) $(PROJECT_SHAVE_LIBS)
	$(ECHO) $(LD) $(ENTRYPOINTS_pp_gauss) $(MVLIBOPT) $(SHAVE_pp_gauss_OBJS) $(filter-out $(CommonMlibFile).mvlib,$(PROJECT_SHAVE_LIBS)) -o $@

