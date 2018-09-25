MDK_BASE = ../../../../../../../..
MV_SOC_REV ?= ma2150

ifeq ($(MV_SOC_REV), ma2150)
 MVSOC         = -DMA2150
else ifeq ($(MV_SOC_REV), ma2155)
 MVSOC         = -DMA2155
else ifeq ($(MV_SOC_REV), ma2450)
 MVSOC         = -DMA2450
else ifeq ($(MV_SOC_REV), ma2455)
 MVSOC         = -DMA2455
endif

SIPP_BASE     = $(MDK_BASE)/common/components/sipp
MDK_MVSOC_INC = $(MDK_BASE)/common/drivers/myriad2/socDrivers/leon/arch/ma2x5x/include

SIPP_PKG_BASE = $(MDK_BASE)/packages/movidius/sipp/arch/ma2x5x
#
# hwModels for MA2150 are contained in a tagged version ????
# here: http://dub30/svn/TAGS/sipp/hwModels/<DATE>
# Before building do an svn export to the HWMODELS_SRC_BASE folder.
#
HWMODELS_SRC_BASE = $(SIPP_BASE)/arch/$(MV_SOC_REV)/hwModels
HWMODELS_PKG_BASE = $(SIPP_PKG_BASE)/hwModels

ifndef UNAME
UNAME := $(shell uname -o)
endif

ifeq "$(UNAME)" "GNU/Linux"
# Linux platform detected. Select appropriate path
LIB_PREBUILD_DIR = $(HWMODELS_PKG_BASE)/lib/linux
else
# Not Linux
LIB_PREBUILD_DIR = $(HWMODELS_PKG_BASE)/lib/cygwin
endif

# Determine whether 32/64
BUILD   ?= $(shell getconf LONG_BIT)

CFLAGS  += -Wall -g -m32 -O1 -fpermissive -Wno-unknown-pragmas -Wno-comment -DMEASURE_TIME -fopenmp $(MVSOC) -DFULL_SIPP_BUILD

LDFLAGS += -Wall $(LIBS) -Wl,-rpath=$(LD_LIBRARY_PATH)
LIBS    += -lpthread -lm

SRCDIR    ?= $(HWMODELS_SRC_BASE)

OBJDIR  := obj

SOURCE    ?= $(SRCDIR)/sources
UTLSOURCE ?= $(SRCDIR)/testUtils
TXSOURCE  ?= $(SOURCE)/sippMipiTxStates
STRSOURCE ?= $(SOURCE)/sippStrategies
DECSOURCE ?= $(SOURCE)/sippDecorators
BLDSOURCE ?= $(SOURCE)/sippBuilders
VISSOURCE ?= $(SOURCE)/sippVisitors
CMXDMASOURCE ?= $(SRCDIR)/cmxdmaSrc
EXTDIR    ?= $(SRCDIR)/external
UTLINCDIR ?= $(SRCDIR)/testUtils
INCDIR    ?= $(SRCDIR)/include
TXINCDIR  ?= $(INCDIR)/sippMipiTxStates
STRINCDIR ?= $(INCDIR)/sippStrategies
DECINCDIR ?= $(INCDIR)/sippDecorators
BLDINCDIR ?= $(INCDIR)/sippBuilders
VISINCDIR ?= $(INCDIR)/sippVisitors
HWCOMDIR  ?= $(SIPP_PKG_BASE)/include/TargetHw/$(MV_SOC_REV)
CMXDMAINCDIR ?= $(SRCDIR)/cmxdmaInc
INCDIRS   ?= -I. -I$(INCDIR) \
                 -I$(UTLINCDIR) \
                 -I$(TXINCDIR) \
                 -I$(STRINCDIR) \
                 -I$(DECINCDIR) \
                 -I$(BLDINCDIR) \
                 -I$(VISINCDIR) \
                 -I$(EXTDIR)/mdkInc \
                 -I$(EXTDIR)/moviToolsCommonInc \
                 -I$(HWCOMDIR) \
                 -I$(CMXDMAINCDIR) \
                 -I$(SRCDIR)/testUtils


OBJS += $(OBJDIR)/moviFloat32.o \
        $(OBJDIR)/measure.o \
        $(OBJDIR)/fp16.o \
        $(OBJDIR)/sippBuffer.o \
        $(OBJDIR)/sippBase.o \
        $(OBJDIR)/sippDebug.o \
        $(OBJDIR)/sippOpipe.o \
        $(OBJDIR)/sippMedian.o \
        $(OBJDIR)/sippSharpen.o \
        $(OBJDIR)/sippPolyphaseFIR.o \
        $(OBJDIR)/sippChromaDenoiseAgg.o \
        $(OBJDIR)/sippChromaGaussian.o \
        $(OBJDIR)/sippChromaDenoise.o \
        $(OBJDIR)/sippGreyDesat.o \
        $(OBJDIR)/sippConvolution.o \
        $(OBJDIR)/sippEdgeOperator.o \
        $(OBJDIR)/sippLookUpTable.o \
        $(OBJDIR)/sippHarrisCorners.o \
        $(OBJDIR)/sippDebayer.o \
        $(OBJDIR)/sippDebayerPPM.o \
        $(OBJDIR)/sippRAW.o \
        $(OBJDIR)/sippLumaDenoise.o \
        $(OBJDIR)/sippLSC.o \
        $(OBJDIR)/sippColorCombination.o \
        $(OBJDIR)/sippDogLtmAgg.o \
        $(OBJDIR)/sippBackgroundGen.o \
        $(OBJDIR)/sippResizeByTwo.o \
        $(OBJDIR)/sippLtmApply.o \
        $(OBJDIR)/sippMultiLineUpscale.o \
        $(OBJDIR)/sippDoG.o \
        $(OBJDIR)/sippDogSubtract.o \
        $(OBJDIR)/sippGenChromaAgg.o \
        $(OBJDIR)/sippResizeByHalf.o \
        $(OBJDIR)/sippPurpleFlare.o \
        $(OBJDIR)/sippDarkAreaDesat.o \
        $(OBJDIR)/sippGenChroma.o \
        $(OBJDIR)/sippSigmaDenoise.o \
        $(OBJDIR)/sippMipiRx.o \
        $(OBJDIR)/sippMipiTx.o \
        $(OBJDIR)/sippHw.o \
        $(OBJDIR)/sippHwFactory.o \
        $(OBJDIR)/sippHwM2Factory.o \
        $(OBJDIR)/sippHwDecorator.o \
        $(OBJDIR)/sippHwR2Decorator.o \
        $(OBJDIR)/sippAccFactory.o \
        $(OBJDIR)/sippAccM2Factory.o \
        $(OBJDIR)/sippThrBoxFilter.o \
        $(OBJDIR)/sippDesaturation.o \
        $(OBJDIR)/sippLumaComputation.o \
        $(OBJDIR)/sippBilinearUpsample.o \
        $(OBJDIR)/sippFilterConsolidator.o \
        $(OBJDIR)/sippUtils.o \
        $(OBJDIR)/sippAddressSpaceLU.o \
        $(OBJDIR)/sippM2AddressSpaceLU.o \
        $(OBJDIR)/sippM2R2AddressSpaceLU.o \
        $(OBJDIR)/sippM2ApbReadVisitor.o \
        $(OBJDIR)/sippM2ApbWriteVisitor.o \
        $(OBJDIR)/sippM2R2ApbReadVisitor.o \
        $(OBJDIR)/sippM2R2ApbWriteVisitor.o \
        $(OBJDIR)/sippM2HwAccIds.o \
        $(OBJDIR)/sippM2R2HwAccIds.o \
        $(OBJDIR)/mipiTxHState.o \
        $(OBJDIR)/mipiTxHSync.o \
        $(OBJDIR)/mipiTxHBackPorch.o \
        $(OBJDIR)/mipiTxActiveWidth.o \
        $(OBJDIR)/mipiTxHFrontPorch.o \
        $(OBJDIR)/mipiTxVState.o \
        $(OBJDIR)/mipiTxVSync.o \
        $(OBJDIR)/mipiTxVBackPorch.o \
        $(OBJDIR)/mipiTxActiveHeight.o \
        $(OBJDIR)/mipiTxVFrontPorch.o \
        $(OBJDIR)/edgeOpAngleStrategy.o \
        $(OBJDIR)/edgeOpSpAngle.o \
        $(OBJDIR)/edgeOpHpAngle.o \
        $(OBJDIR)/harrisOutputStrategy.o \
        $(OBJDIR)/harrisOutputScore.o \
        $(OBJDIR)/harrisOutputSelective.o \
        $(OBJDIR)/harrisAccumulation.o \
        $(OBJDIR)/harrisGaussianSmoothing.o \
        $(OBJDIR)/convolutionCoeffStrategy.o \
        $(OBJDIR)/convolutionNormalCoeffs.o \
        $(OBJDIR)/convolutionIntrlvdCoeffs.o \
        $(OBJDIR)/rawGainStrategy.o \
        $(OBJDIR)/rawCircularGains.o \
        $(OBJDIR)/rawFlexibleGains.o \
        $(OBJDIR)/rawStrategy.o \
        $(OBJDIR)/rawStrategyRev1.o \
        $(OBJDIR)/rawStrategyRev2.o \
        $(OBJDIR)/rawPatchStats.o \
        $(OBJDIR)/rawPatchStatsRev1.o \
        $(OBJDIR)/rawPatchStatsRev2.o \
        $(OBJDIR)/medianNoBlend.o \
        $(OBJDIR)/medianLumaAlphaBlend.o \
        $(OBJDIR)/sigmaBayer.o \
        $(OBJDIR)/sigmaPlanar.o \
        $(OBJDIR)/ccSyncFillControl.o \
        $(OBJDIR)/ccAsyncFillControl.o \
        $(OBJDIR)/upfirdn16bitCoeffs.o \
        $(OBJDIR)/upfirdn8bitCoeffs.o \
        $(OBJDIR)/dbyrStrategyRev2.o \
        $(OBJDIR)/dbyrStrategy.o \
        $(OBJDIR)/dataPathIbflIrq.o \
        $(OBJDIR)/yDnsNonClampedOutput.o \
        $(OBJDIR)/yDnsClampedOutput.o \
        $(OBJDIR)/lineLogicCrop.o \
        $(OBJDIR)/lineLogicPad.o \
        $(OBJDIR)/llbCopyIbflIrq.o \
        $(OBJDIR)/paddingBayer.o \
        $(OBJDIR)/paddingPlanar.o \
        $(OBJDIR)/lutLookUpOutput.o \
        $(OBJDIR)/lutCscOutput.o \
        $(OBJDIR)/lutInstantLoad.o \
        $(OBJDIR)/lutEofLoad.o \
        $(OBJDIR)/rawHist.o \
        $(OBJDIR)/rawHistRev1.o \
        $(OBJDIR)/rawHistRev2.o \
        $(OBJDIR)/yDnsBasicRefKernel.o \
        $(OBJDIR)/yDnsEnhancedRefKernel.o \
        $(OBJDIR)/ccDesaturatedRgb.o \
        $(OBJDIR)/ccNormalRgb.o \
        $(OBJDIR)/customLlbFrontEnd.o \
        $(OBJDIR)/llbFrontEnd.o \
        $(OBJDIR)/cmxbFrontEnd.o \
        $(OBJDIR)/coarseGrainedDumper.o \
        $(OBJDIR)/fineGrainedDumper.o \
        $(OBJDIR)/imageDumper.o \
        $(OBJDIR)/ccBasicCcm.o \
        $(OBJDIR)/ccEnhancedCcm.o \
        $(OBJDIR)/compositeBuilder.o \
        $(OBJDIR)/genChromaM2R2Builder.o \
        $(OBJDIR)/chromaDenoiseM2R1Builder.o \
        $(OBJDIR)/chromaDenoiseM2R2Builder.o \
        $(OBJDIR)/dogLtmM2R2Builder.o

OBJS += $(OBJDIR)/CmxDma.o \
        $(OBJDIR)/CmxDmaArbiter.o \
        $(OBJDIR)/CmxDmaScheduler.o

HDRS += $(EXTDIR)/moviToolsCommonInc/moviFloat32.h \
        $(EXTDIR)/mdkInc/registersMyriad2v2.h \
        $(INCDIR)/measure.h \
        $(INCDIR)/fp16.h \
        $(INCDIR)/criticalSection.h \
        $(INCDIR)/sippCommon.h \
        $(INCDIR)/DrvSippDefines.h \
        $(INCDIR)/sippBuffer.h \
        $(INCDIR)/sippBase.h \
        $(INCDIR)/sippBaseAgg.h \
        $(INCDIR)/sippBaseSu.h \
        $(INCDIR)/sippDebug.h \
        $(INCDIR)/sippOpipe.h \
        $(INCDIR)/sippMedian.h \
        $(INCDIR)/sippSharpen.h \
        $(INCDIR)/sippPolyphaseFIR.h \
        $(INCDIR)/sippChromaDenoiseAgg.h \
        $(INCDIR)/sippChromaGaussian.h \
        $(INCDIR)/sippChromaDenoise.h \
        $(INCDIR)/sippGreyDesat.h \
        $(INCDIR)/sippConvolution.h \
        $(INCDIR)/sippEdgeOperator.h \
        $(INCDIR)/sippLookUpTable.h \
        $(INCDIR)/sippHarrisCorners.h \
        $(INCDIR)/sippDebayer.h \
        $(INCDIR)/sippDebayerPPM.h \
        $(INCDIR)/sippRAW.h \
        $(INCDIR)/sippLumaDenoise.h \
        $(INCDIR)/sippLSC.h \
        $(INCDIR)/sippColorCombination.h \
        $(INCDIR)/sippDogLtmAgg.h \
        $(INCDIR)/sippBackgroundGen.h \
        $(INCDIR)/sippResizeByTwo.h \
        $(INCDIR)/sippLtmApply.h \
        $(INCDIR)/sippMultiLineUpscale.h \
        $(INCDIR)/sippDoG.h \
        $(INCDIR)/sippDogSubtract.h \
        $(INCDIR)/sippGenChromaAgg.h \
        $(INCDIR)/sippResizeByHalf.h \
        $(INCDIR)/sippPurpleFlare.h \
        $(INCDIR)/sippDarkAreaDesat.h \
        $(INCDIR)/sippGenChroma.h \
        $(INCDIR)/sippSigmaDenoise.h \
        $(INCDIR)/sippMipiRx.h \
        $(INCDIR)/sippMipiTx.h \
        $(INCDIR)/sippMipiInterface.h \
        $(INCDIR)/sippIrq.h \
        $(INCDIR)/sippHwInterface.h \
        $(INCDIR)/sippHw.h \
        $(INCDIR)/sippHwFactory.h \
        $(INCDIR)/sippHwM2Factory.h \
        $(INCDIR)/sippHwDecorator.h \
        $(INCDIR)/sippHwR2Decorator.h \
        $(INCDIR)/sippAccFactory.h \
        $(INCDIR)/sippAccM2Factory.h \
        $(INCDIR)/sippAccM2.h \
        $(INCDIR)/sippBitManipulation.h \
        $(INCDIR)/sippNoPad.h \
        $(INCDIR)/sippThrBoxFilter.h \
        $(INCDIR)/sippDesaturation.h \
        $(INCDIR)/sippLumaComputation.h \
        $(INCDIR)/sippBilinearUpsample.h \
        $(INCDIR)/sippLutLoadMechanism.h \
        $(INCDIR)/sippFilterConsolidator.h \
        $(INCDIR)/sippUtils.h \
        $(INCDIR)/sippAddressSpaceLU.h \
        $(INCDIR)/sippM2AddressSpaceLU.h \
        $(INCDIR)/sippM2R2AddressSpaceLU.h \
        $(INCDIR)/sippM2HwAccIds.h \
        $(INCDIR)/sippM2R2HwAccIds.h \
        $(INCDIR)/sippTestDefines.h \
        $(VISINCDIR)/sippVisitor.h \
        $(VISINCDIR)/sippM2ApbReadVisitor.h \
        $(VISINCDIR)/sippM2ApbWriteVisitor.h \
        $(VISINCDIR)/sippM2R2ApbReadVisitor.h \
        $(VISINCDIR)/sippM2R2ApbWriteVisitor.h \
        $(TXINCDIR)/mipiTxState.h \
        $(TXINCDIR)/mipiTxHState.h \
        $(TXINCDIR)/mipiTxHSync.h \
        $(TXINCDIR)/mipiTxHBackPorch.h \
        $(TXINCDIR)/mipiTxActiveWidth.h \
        $(TXINCDIR)/mipiTxHFrontPorch.h \
        $(TXINCDIR)/mipiTxVState.h \
        $(TXINCDIR)/mipiTxVSync.h \
        $(TXINCDIR)/mipiTxVBackPorch.h \
        $(TXINCDIR)/mipiTxActiveHeight.h \
        $(TXINCDIR)/mipiTxVFrontPorch.h \
        $(STRINCDIR)/edgeOpAngleStrategy.h \
        $(STRINCDIR)/edgeOpSpAngle.h \
        $(STRINCDIR)/edgeOpHpAngle.h \
        $(STRINCDIR)/edgeOpOctantChoice.h \
        $(STRINCDIR)/harrisOutputStrategy.h \
        $(STRINCDIR)/harrisOutputScore.h \
        $(STRINCDIR)/harrisOutputSelective.h \
        $(STRINCDIR)/harrisOutputComputation.h \
        $(STRINCDIR)/harrisStructureTensorStrategy.h \
        $(STRINCDIR)/harrisAccumulation.h \
        $(STRINCDIR)/harrisGaussianSmoothing.h \
        $(STRINCDIR)/convolutionCoeffStrategy.h \
        $(STRINCDIR)/convolutionNormalCoeffs.h \
        $(STRINCDIR)/convolutionIntrlvdCoeffs.h \
        $(STRINCDIR)/convolutionCoeffSelection.h \
        $(STRINCDIR)/medianBlendStrategy.h \
        $(STRINCDIR)/medianNoBlend.h \
        $(STRINCDIR)/medianLumaAlphaBlend.h \
        $(STRINCDIR)/medianBlendLogic.h \
        $(STRINCDIR)/lutOutputStrategy.h \
        $(STRINCDIR)/lutLookUpOutput.h \
        $(STRINCDIR)/lutCscOutput.h \
        $(STRINCDIR)/lutOutputLogic.h \
        $(STRINCDIR)/lutLoadStrategy.h \
        $(STRINCDIR)/lutInstantLoad.h \
        $(STRINCDIR)/lutEofLoad.h \
        $(STRINCDIR)/rawGainStrategy.h \
        $(STRINCDIR)/rawCircularGains.h \
        $(STRINCDIR)/rawFlexibleGains.h \
        $(STRINCDIR)/rawGainSelection.h \
        $(STRINCDIR)/rawStrategy.h \
        $(STRINCDIR)/rawStrategyRev1.h \
        $(STRINCDIR)/rawStrategyRev2.h \
        $(STRINCDIR)/rawPatchStats.h \
        $(STRINCDIR)/rawPatchStatsRev1.h \
        $(STRINCDIR)/rawPatchStatsRev2.h \
        $(STRINCDIR)/medianBlendStrategy.h \
        $(STRINCDIR)/medianNoBlend.h \
        $(STRINCDIR)/medianLumaAlphaBlend.h \
        $(STRINCDIR)/medianBlendLogic.h \
        $(STRINCDIR)/sigmaDataPathStrategy.h \
        $(STRINCDIR)/sigmaBayer.h \
        $(STRINCDIR)/sigmaPlanar.h \
        $(STRINCDIR)/sigmaDataPathParams.h \
        $(STRINCDIR)/ccFillControlStrategy.h \
        $(STRINCDIR)/ccSyncFillControl.h \
        $(STRINCDIR)/ccAsyncFillControl.h \
        $(STRINCDIR)/upfirdnCoeffStrategy.h \
        $(STRINCDIR)/upfirdn16bitCoeffs.h \
        $(STRINCDIR)/upfirdn8bitCoeffs.h \
        $(STRINCDIR)/dbyrStrategy.h \
        $(STRINCDIR)/dbyrStrategyRev2.h \
        $(STRINCDIR)/yDnsOutputStrategy.h \
        $(STRINCDIR)/yDnsNonClampedOutput.h \
        $(STRINCDIR)/yDnsClampedOutput.h \
        $(STRINCDIR)/yDnsOutputLogic.h \
        $(STRINCDIR)/lineLogicStrategy.h \
        $(STRINCDIR)/lineLogicCrop.h \
        $(STRINCDIR)/lineLogicPad.h \
        $(STRINCDIR)/paddingStrategy.h \
        $(STRINCDIR)/paddingBayer.h \
        $(STRINCDIR)/paddingPlanar.h \
        $(STRINCDIR)/dataPathIbflIrq.h \
        $(STRINCDIR)/llbCopyIbflIrq.h \
        $(STRINCDIR)/ibflIrqControlStrategy.h \
        $(DECINCDIR)/rawHist.h \
        $(DECINCDIR)/rawHistRev1.h \
        $(DECINCDIR)/rawHistRev2.h \
        $(DECINCDIR)/histogram.h \
        $(DECINCDIR)/yDnsRefKernel.h \
        $(DECINCDIR)/yDnsBasicRefKernel.h \
        $(DECINCDIR)/yDnsEnhancedRefKernel.h \
        $(DECINCDIR)/yDnsRefKernelBuild.h \
        $(DECINCDIR)/ccRgbConversion.h \
        $(DECINCDIR)/ccDesaturatedRgb.h \
        $(DECINCDIR)/ccNormalRgb.h \
        $(DECINCDIR)/ccRgbComputation.h \
        $(DECINCDIR)/ccCcmApplication.h \
        $(DECINCDIR)/ccBasicCcm.h \
        $(DECINCDIR)/ccEnhancedCcm.h \
        $(DECINCDIR)/ccCcmParameters.h \
        $(DECINCDIR)/bufferManagement.h \
        $(DECINCDIR)/customLlbFrontEnd.h \
        $(DECINCDIR)/llbFrontEnd.h \
        $(DECINCDIR)/cmxbFrontEnd.h \
        $(BLDINCDIR)/compositeBuilder.h \
        $(BLDINCDIR)/genChromaM2R2Builder.h \
        $(BLDINCDIR)/chromaDenoiseM2R1Builder.h \
        $(BLDINCDIR)/chromaDenoiseM2R2Builder.h \
        $(BLDINCDIR)/dogLtmM2R2Builder.h

HDRS += $(CMXDMAINCDIR)/CmxDma.h \
        $(CMXDMAINCDIR)/CmxDmaArbiter.h \
        $(CMXDMAINCDIR)/CmxDmaIrq.h \
        $(CMXDMAINCDIR)/CmxDmaScheduler.h


$(OBJDIR)/%.o: %.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(UTLSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(UTLSOURCE)/%.c
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TXSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(STRSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(DECSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(BLDSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(VISSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(EXTDIR)/moviToolsCommonSrc/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SOURCE)/%.c
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(UTLSOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(CMXDMASOURCE)/%.cpp
	$(CC) $(INCDIRS) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@echo "src dir = $(SRCDIR)"
	@mkdir -p $(OBJDIR)

all:    $(OBJDIR) libhwmodels.a
	cp libhwmodels.a $(LIB_PREBUILD_DIR)

libhwmodels.a:  $(OBJS)
	@echo "$(BUILD) bit build of $(TARGET_CHIP_VER)..."
	ar r libhwmodels.a $(OBJS)

clean:
	$(call SAFE_RM,$(OBJDIR))
	rm -f libhwmodels.a

MDK_PROJ_STRING ?= "MDK_FALSE"
TEST_TAGS := "MA2150"
getTestType:

isMdkProject:
	@echo $(MDK_PROJ_STRING)
