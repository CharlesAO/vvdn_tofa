PIPE_NAME = ispFromRx

#######################################################################
# WARNING: PlgSource below is just included to avoid compile errors due
#          to inclusion of "PlgSourceApi.h" in "ipipeOpipeUtils.c"
#######################################################################

#assumed preexisting define on upper layer
ComponentList_LRT += Opipe      \
                     UnitTestVcs \
                     VcsHooks    \
                     $(IPIPE_COMPONENTS)/IpipeMsgQueue \
                     $(IPIPE_COMPONENTS)/FrameMgr      \
                     $(IPIPE_COMPONENTS)/MemMgr        \
                     $(IPIPE_COMPONENTS)/IpipeServer   \
                     $(IPIPE_COMPONENTS)/common     \
                     $(IPIPE_PLUGINS_BASE_ARCH)/common/IspCommon \
                     $(IPIPE_PLUGINS_BASE_ARCH)/PlgSrcIsp        \
                     $(IPIPE_PLUGINS_BASE_ARCH)/PlgSource        \
                     $(IPIPE_COMPONENTS)/pipelines/appUtils   \
                     $(IPIPE_COMPONENTS)/pipelines/$(PIPE_NAME)

#opipe extra defines
#CCOPT_LRT += -D'OPIPE_RUNTIME_CHECKS'

LEON_RT_LIB_NAME = $(IPIPE_BASE)/components/pipelines/$(PIPE_NAME)/leonRTApp
