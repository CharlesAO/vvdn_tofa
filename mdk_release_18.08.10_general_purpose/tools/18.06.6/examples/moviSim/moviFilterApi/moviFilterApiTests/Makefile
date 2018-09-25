#include ../../common.mk


compilerFlags.std.c++        ?= $(compilerFlags.c++11)

compilerFlags.common         ?= -pipe
linkerFlags.common           ?= -pipe

compilerFlags.32             ?= -m32 -march=i686
linkerFlags.32               ?= -m32
compilerFlags.64             ?= -m64
linkerFlags.64               ?= -m64

linkerFlags             += -lrt -ldl
linkerFlags64           += -lrt -ldl

compiler                     ?= g++
compiler.c                   ?= gcc
linker                       ?= g++

buildConfig = release64

moviFilterApiTests = moviFilterApiTests.elf
moviFilterApiTestsFoler = .
moviBaseLibFolder       = $(moviFilterApiTestsFoler)/../../../moviBaseLib/Binaries/Release
moviBaseLibrary         = moviBaseLib.a

releaseFolder           = Binaries/Release
debugFolder             = Binaries/Debug


VPATH = $(moviFilterApiTestsFoler)/Sources

includes = \
    -IIncludes \
    -I$(moviFilterApiTestsFoler)/Includes \
    -I$(moviFilterApiTestsFoler)/../Includes \
    -I$(moviFilterApiTestsFoler)/../../../moviBaseLib/Includes

moviFilterApiTestsSources = \
    dpuFilterApiTest.cpp \
    warpResources.cpp \
    filterApiTestsUtils.cpp \
    stereoResources.cpp \
    stereoFilterApiTest.cpp \
    warpFilterApiTest.cpp \
    moviFilterApiTests.cpp

moviFilterApiTestsObjectsRelease = $(addprefix $(moviFilterApiTestsFoler)/$(releaseFolder)/, $(addsuffix .o, $(basename $(moviFilterApiTestsSources))))
#Rules:
all: release64

release64: buildConfig = release64
release64: moviFilterApiTestsRelease
	$(ECHO)$(STRIP) -- $(moviFilterApiTestsFoler)/$(releaseFolder)/$(moviFilterApiTests)

release64: compilerFlags=$(compilerFlags64) -fpermissive -std=c++11
release64: linkerFlags=$(linkerFlags64) $(coverageLinkerFlags)

moviFilterApiTestsRelease: $(moviFilterApiTestsObjectsRelease) 
	@echo Linking $(moviFilterApiTests)
	$(ECHO)$(linker) $(moviFilterApiTestsObjectsRelease) \
               $(moviBaseLibFolder)/$(moviBaseLibrary) \
               -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(moviFilterApiTests) $(linkerFlags)

$(moviFilterApiTestsFoler)/$(releaseFolder)/%.o: %.cpp
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsRelease) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(notdir $(@))

$(moviFilterApiTestsFoler)/$(releaseFolder)/%.o: %.c
	@echo Compiling for $(buildConfig): $<
	$(ECHO)$(compiler) $(compilerFlags) $(compilerFlagsRelease) $(includes) -c $< -o $(moviFilterApiTestsFoler)/$(releaseFolder)/$(notdir $(@))

clean:
	@echo Cleaning objects...
	-@$(RM) $(moviFilterApiTestsFoler)/$(releaseFolder)/*.*
	@echo Cleaning complete

cleanall: clean
	@echo Cleaning library objects...
	@+make -C $(moviBaseLibFolder) clean
	@echo Clean all complete
