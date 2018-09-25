ComponentList_SVE += profiler
SHAVE_COMPONENTS = yes
#MVCCOPT += -fprofile-instr-generate -fcoverage-mapping -DFS_ENABLE
MVCCOPT += -fprofile-instr-generate -DFS_ENABLE

ifdef FS_TOTAL_SIZE
	MVCCOPT += -DFS_TOTAL_SIZE=$(FS_TOTAL_SIZE)
endif
# to include our open/write implementation and for the linker not to remove .ctors
MVLIBOPT += -u write -u open -u _write -u _open -u initFS -u __profilerInit
PROJECT_SHAVE_LIBS += ${MV_TOOLS_DIR}/${MV_TOOLS_VERSION}/common/moviCompile/lib/${MVISAOPT}/mlibprofile.a
