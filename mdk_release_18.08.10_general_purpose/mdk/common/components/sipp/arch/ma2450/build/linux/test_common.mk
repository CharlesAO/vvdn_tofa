MDK_PROJ_STRING ?= "MDK_TRUE"

isMdkProject:
	@echo $(MDK_PROJ_STRING)
getTestType:
	@echo "TEST_TYPE=$(TEST_TYPE) TEST_TIMEOUT_SEC=$(TEST_TIMEOUT_SEC) TEST_TIMEOUT_LEON_INST=$(TEST_TIMEOUT_LEON_INST) TEST_HW_PLATFORM=$(TEST_HW_PLATFORM) TEST_TAGS=$(TEST_TAGS) TEST_TOOLS=$(MV_TOOLS_VERSION)"

