if(EXISTS "$ENV{MV_COMMON_BASE}")
    set(MV_COMMON_BASE $ENV{MV_COMMON_BASE})
else()
    set(MV_COMMON_BASE ${CMAKE_CURRENT_LIST_DIR}/../..)
endif(EXISTS "$ENV{MV_COMMON_BASE}")

#set Directories with vTrack Components
set(vTrackDir ${CMAKE_CURRENT_LIST_DIR}/source)
set(mestCompDir ${MV_COMMON_BASE}/components/MEST)
include(${mestCompDir}/mest.cmake)
set(mestDirApi ${MV_COMMON_BASE}/components/MEST/api)
set(vTrackApi ${vTrackDir}/api)
set(vTrackResultsApi ${vTrackDir}/apiLite)
set(vTrackCommon ${vTrackDir}/common)
set(ofSourcesLeonDir ${vTrackDir}/opticalFlow/leon)
set(ofSourcesSharedDir ${vTrackDir}/opticalFlow/shared)
set(ofSourcesShaveDir ${vTrackDir}/opticalFlow/shvDynApps/of)
set(ofSourcesPcDir ${vTrackDir}/opticalFlow/pc)
set(fmSourcesLeonDir ${vTrackDir}/featurePipe/leon)
set(fmSourcesSharedDir ${vTrackDir}/featurePipe/shared)
set(fmSourcesPcDir ${vTrackDir}/featurePipe/pc)
set(fmSourcesShaveDir ${vTrackDir}/featurePipe/shvDynApps/fm)
set(ppSourcesLeonDir ${vTrackDir}/pixelPipe/leon)
set(ppSourcesSharedDir ${vTrackDir}/pixelPipe/shared)
set(ppSourcesPcDir ${vTrackDir}/pixelPipe/pc)
set(vpSourcesLeonDir ${vTrackDir}/vpipe/leon) #TODO: standardise cases!
set(vpSourcesPcDir ${vTrackDir}/vpipe/pc)
set(drawingSourcesPcDir ${vTrackDir}/utilities/pc)
set(mestSourcesPcDir ${mestDirApiPc}/)
set(mestSourcesSharedDir ${mestDirApiShared}/)

#find source files in vTrack component directories )
FILE(GLOB vTrackSharedSrc ${vTrackCommon}/*.cpp)
FILE(GLOB ofSources_pc ${ofSourcesPcDir}/*.cpp)
FILE(GLOB fmSources_pc ${fmSourcesPcDir}/*.c ${fmSourcesPcDir}/*.cpp)
FILE(GLOB fmSources_shave ${fmSourcesShaveDir}/*.cpp)
FILE(GLOB ppSources_leon ${ppSourcesLeonDir}/pixelPipeCommon.cpp)
FILE(GLOB ppSources_pc ${ppSourcesPcDir}/*.cpp ${ppSourcesPcDir}/*.c)
FILE(GLOB vpSources_leon ${vpSourcesLeonDir}/*.cpp ${vpSourcesLeonDir}/*.c)
FILE(GLOB vpSources_pc ${vpSourcesPcDir}/*.cpp ${vpSourcesPcDir}/*.c)
FILE(GLOB mestSources_shared ${mestSourcesSharedDir}/*.cpp ${mestSourcesSharedDir}/*.c)
FILE(GLOB mestSources_pc ${mestSourcesPcDir}/*.cpp ${mestSourcesPcDir}/*.c)
FILE(GLOB drawingSources ${drawingSourcesPcDir}/*.cpp)

set(VTRACK_PUBLIC_HEADERS
    ${vTrackResultsApi}/vTrackOutput.h
    ${vTrackApi}/vTrack.h
    ${vTrackApi}/vPipe.h
    ${vTrackApi}/vPipePublicTypes.h
    ${vTrackApi}/global_constants.h
    ${vTrackDir}/pc/pc_constants.h
)

set(VTRACK_MINIMAL_INCLUDE_DIRECTORIES
    ${vTrackResultsApi}
)

set(VTRACK_INCLUDE_DIRECTORIES
    ${VTRACK_MINIMAL_INCLUDE_DIRECTORIES}
    ${vTrackApi}
    ${vTrackCommon}
    ${ofSourcesLeonDir}
    ${ofSourcesSharedDir}
    ${ofSourcesShaveDir}
    ${ofSourcesPcDir}
    ${MV_COMMON_BASE}/components/vTrack/opticalFlow/shvDynApps/of
    ${fmSourcesLeonDir}
    ${fmSourcesSharedDir}
    ${fmSourcesShaveDir}
    ${ppSourcesLeonDir}
    ${ppSourcesPcDir}
    ${ppSourcesSharedDir}
    ${vpSourcesLeonDir}
    ${vpSourcesPcDir}
    ${drawingSourcesPcDir}
    ${mestSourcesSharedDir}
    ${mestSourcesPcDir}
    ${vTrackDir}/pc
    ${MV_COMMON_BASE}/shared/include
    ${MV_COMMON_BASE}/swCommon/pcModel/dmaModel
    ${MV_COMMON_BASE}/swCommon/pcModel/half
    ${MV_COMMON_BASE}/swCommon/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/opticalFlowPyrLK/shave/include
    ${MV_COMMON_BASE}/../packages/movidius/pcModel/moviCompile/compilerVectorFunctions/include
    ${MV_COMMON_BASE}/../packages/movidius/pcModel/moviCompile/compilerIntrinsics/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/calcG/shave/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/calcBxBy/shave/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/bilinearInterpolation_u16/shave/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/bilinearInterpolationAligned_u16/shave/include
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/conv3x3fp32Scharr/shave/include
)

set(VTRACK_SOURCES
    ${mainSources}
    ${vTrackSharedSrc}
    ${ofSources_pc}
    ${fmSources_pc}
    ${fmSources_shave}
    ${ppSources_leon}
    ${ppSources_pc}
    ${vpSources_leon}
    ${vpSources_pc}
    ${mestSources_shared}
    ${mestSources_pc}
    ${MV_COMMON_BASE}/swCommon/src/swcLeonMath.c
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/opticalFlowPyrLK/shave/src/cpp/opticalFlowPyrLK.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/opticalFlowPyrLK/shave/src/cpp/samplers.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/shave/mat.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/shave/mvcv_types.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/calcG/shave/src/cpp/calcG.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/calcBxBy/shave/src/cpp/calcBxBy.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/bilinearInterpolation_u16/shave/src/cpp/bilinearInterpolation_u16.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/bilinearInterpolationAligned_u16/shave/src/cpp/bilinearInterpolationAligned_u16.cpp
    ${MV_COMMON_BASE}/components/kernelLib/MvCV/kernels/conv3x3fp32Scharr/shave/src/cpp/conv3x3fp32Scharr.cpp
)

set(VTRACK_DRAWING_SOURCES
    ${drawingSources}
)

set(VTRACK_DRAWING_PUBLIC_HEADERS
    ${drawingSourcesPcDir}/featuresDrawer.h
    ${drawingSourcesPcDir}/graphPlotter.h
)

add_library(libmest STATIC IMPORTED)
set_target_properties(libmest  PROPERTIES IMPORTED_LOCATION ${mestDirApiPc}/libmest.a)

set(VTRACK_LIBS
    libmest
)


