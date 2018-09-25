include ../common.mk

linkerFlags             += -lrt
linkerFlags64           += -lrt

buildConfig = release64

moviFilterApiTests = moviFilterApiTests.elf
moviFilterApiTestsFoler = moviFilterApiTests

VPATH = $(moviFilterApiTestsFoler)/Sources

includes = \
    -IIncludes \
    -I$(moviFilterApiTestsFoler)/Includes \
    -I../../moviBaseLib/Includes

moviFilterApiTestsSources = \
    dpuFilterApiTest.cpp \
    stereoResources.cpp \
    stereoFilterApiTest.cpp \
    warpResources.cpp \
    warpFilterApiTest.cpp \
    filterApiTestsUtils.cpp \
    moviFilterApiTests.cpp

moviFilterApiTestsObjectsRelease = $(addprefix $(moviFilterApiTestsFoler)/$(releaseFolder)/, $(addsuffix .o, $(basename $(moviFilterApiTestsSources))))
moviFilterApiTestsObjectsDebug = $(addprefix $(moviFilterApiTestsFoler)/$(debugFolder)/, $(addsuffix .o, $(basename $(moviFilterApiTestsSources))))
#Rules:
all: release64

release64: buildConfig = release64
release64: moviFilterApiTestsRelease
	$(ECHO)$(STRIP) -- $(moviFilterApiTestsFoler)/$(releaseFolder)/$(moviFilterApiTests)

debug64: buildConfig = debug64
debug64: moviFilterApiTestsDebug

debug64 release64: compilerFlags=$(compilerFlags64) $(coverageCompilerFlags)
debug64 release64: linkerFlags=$(linkerFlags64) $(coverageLinkerFlags)

moviFilterApiTestsRelease: $(moviFilterApiTestsObjectsRelease) moviBaseLibRelease
	@echo Linking $(moviFilterApiTests)
	$(ECHO)$(linker) $(moviFilterApiTestsObjectsRelease) \
               $(moviBaseLibFolder)/$(releaseFolder)/$(moviBaseLibrary) \
               -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(moviFilterApiTests) $(linkerFlags)

moviFilterApiTestsDebug: $(moviFilterApiTestsObjectsDebug) moviBaseLibDebug
	@echo Linking $(moviFilterApiTests)
	$(ECHO)$(linker) $(moviFilterApiTestsObjectsDebug) \
               $(moviBaseLibFolder)/$(debugFolder)/$(moviBaseLibrary) \
               -o $(moviFilterApiTestsFoler)/$(debugFolder)/$(moviFilterApiTests) $(linkerFlags)

moviBaseLibRelease moviBaseLibDebug:
	@echo Building moviBaseLib...
	+make -C $(moviBaseLibFolder) $(buildConfig)

$(moviFilterApiTestsFoler)/$(releaseFolder)/%.o: %.cpp
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsRelease) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(notdir $(@))

$(moviFilterApiTestsFoler)/$(releaseFolder)/%.o: %.c
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsRelease) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(notdir $(@))

$(moviFilterApiTestsFoler)/$(debugFolder)/%.o: %.cpp
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsDebug) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(debugFolder)/$(notdir $(@))

$(moviFilterApiTestsFoler)/$(debugFolder)/%.o: %.c
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsDebug) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(debugFolder)/$(notdir $(@))

clean:
	@echo Cleaning objects...
	-@$(RM) $(moviFilterApiTestsFoler)/$(releaseFolder)/*.*
	-@$(RM) $(moviFilterApiTestsFoler)/$(debugFolder)/*.*
	@echo Cleaning complete

cleanall: clean
	@echo Cleaning library objects...
	@+make -C $(moviBaseLibFolder) clean
	@echo Clean all complete
