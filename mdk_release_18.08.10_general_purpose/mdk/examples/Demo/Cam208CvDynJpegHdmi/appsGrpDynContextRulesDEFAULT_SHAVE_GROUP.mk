
define cGroupAppVars_templateDEFAULT_SHAVE_GROUP

cDynContextDefs_$(1) += -D'APPGROUPDYNDATASECTION=DEFAULT_SHAVE_GROUPgrpdyndata'
cDynContextDefs_$(1) += -D'APPGROUPDYNBSSDATASECTION=__DEFAULT_SHAVE_GROUP_groupbss_sectionStart'
cDynContextDefs_$(1) += -D'APPGROUPDYNBSSDATASECTIONEND=__DEFAULT_SHAVE_GROUP_groupbss_sectionEnd'
cDynContextDefs_$(1) += -D'GRPENTRY=DEFAULT_SHAVE_GROUP____AllTimeEntryPoint'
cDynContextDefs_$(1) += -D'APPIATGROUP=DEFAULT_SHAVE_GROUP__iat_array_start'
cDynContextDefs_$(1) += -D'APPIATNAMESGROUP=DEFAULT_SHAVE_GROUP__iatnames_array_start'

$(1)_GROUP_DATA := ./output/DEFAULT_SHAVE_GROUP.shvZdata

endef
$(foreach appX,$(MV_DEFAULT_SHAVE_GROUP_APPS),$(eval $(call cGroupAppVars_templateDEFAULT_SHAVE_GROUP,$(call basenamenotdir,$(appX)))))
	
