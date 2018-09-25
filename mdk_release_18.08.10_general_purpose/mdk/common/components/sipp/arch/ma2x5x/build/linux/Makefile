LIB_DIRS = HwModels SwSipp
MDK_PROJ_STRING ?= "MDK_FALSE"
TEST_TAGS := "MA2150"
getTestType:

isMdkProject:
	@echo $(MDK_PROJ_STRING)
all:
	for dir in $(LIB_DIRS); do \
		$(MAKE) -C $$dir all || exit 1; \
	done

clean:
	for dir in $(LIB_DIRS); do \
		$(MAKE) -C $$dir clean || exit 1; \
	done


