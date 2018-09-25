///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef __SIPP_HWDEFS_H__
#define __SIPP_HWDEFS_H__


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "sippHwDefs_ma2x5x.h"
#endif

#if defined(MA2480) || defined(MA2485)
#include "sippISPDefs_ma2x8x.h"
#include "sippCVDefs_ma2x8x.h"
#endif

//===================================================================
//Dma params

/// @defgroup dma DMA
/// @brief DMA In/Out filter.
/// @ingroup SIPP_Input-Output_Filters
/// @par Output data type(s):\n
///      UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, half, fp16, fp32
/// @par Filter function:\n
///      SIPP_DMA_ID
/// @par Inputs:\n
/// 	- datatypes: UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, half, fp16, fp32; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref dma filter.
typedef struct
{
    /// @brief Private member. Myriad2 DMA 2D-chunked descriptor, as in CMXDMA_controller.doc.
    DmaDesc dmaDsc;

    /// @brief User level params to customize transfer.
    UInt32 ddrAddr;

    #if defined MA2150 || defined MA2450
    /// @brief Private member.
    UInt32 dstChkW;                       // Unused in 2x8x
    /// @brief Private member.
    UInt32 srcChkW;                       // Unused in 2x8x
    /// @brief Private member.
    UInt32 dstChkS;                       // Unused in 2x8x
    /// @brief Private member.
    UInt32 srcChkS;                       // Unused in 2x8x
    /// @brief Private member.
    UInt32 srcPlS;                        // Unused in 2x8x
    #endif

    /// @brief Private member.
    UInt32 dstPlS;
    /// @brief Private member. Full line strides
    UInt32 dstLnS;
    /// @brief Private member. Full line strides
    UInt32 srcLnS;

    /// @brief Private member. Myriad2 DMA 2D-chunked descriptor pointer used when multiple lines per iteration, as in CMXDMA_controller.doc.
    DmaDesc *      pLineDesList;

    /// @brief Allow dma filter to be configured for standard and smart modes
    eDmaMode       dmaMode;
    /// @brief dmaMode specific config information
    DMAExtCfg      extCfg;

    /// @brief bChunked specifies if buffer is chunked or not
    bool           bChunked;

    #if defined MA2480
    /// @brief Private member. Total number of descriptors in the linked list for this filter
    UInt32         numDescriptors;
    /// @brief Private member. Destination line length
    UInt32         dstLnL;

    DMATransDesc * pTransList;
    #endif
    /// @brief Private member. A transaction list used to feed CDMA driver with direct streaming runtime

    #if defined MA2150 || defined MA2450
    #if defined SIPP_SCRT_ENABLE_OPIPE
    DMATransDesc * pTransList;
    #endif
    #endif

    #if defined SIPP_USE_CMXDMA_DRIVER
    #if defined MA2480
    DrvCmxDmaTransaction    * drvDesc;
    DrvCmxDmaTransactionHnd * handle;
    #else
    dmaTransactionList      * drvDesc;
    #endif
    #endif

} DmaParam;
/// @}

//===================================================================
// DMA In/Out filter description for the SIPP Graph Designer Eclipse plugin
// DMA filter has to be described in a special way (not as the rest of the filters) as it has special behaviour
/*{
filter({
 id : "dmaIn",
 name : "DMA In",
 description : "DMA in filter",
 image : "51",

 color : "9090b0",
 shape : "rectangle",
 group : "Input-Output Filters",

 type : "hw",
 symbol :"SIPP_DMA_ID",
 preserve : "",
 dataType : "UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, half, fp16, fp32, float",
 structure :"DmaParam",
 flags : "0x00",

 sourceConstraint : "assert(true,'')",
 targetConstraint : "assert(false,'No input')",
 mandatoryInputs : 0,

 parameters : [ {
  id : "internal0",
  name : "ddrAddr",
  description : "Buffer address to DMA into. If defined as 'auto', the buffer will be automatically generated in DDR.",
  value : "auto",
  type : "u32",
  constraint : "",
 },{
  id : "internal1",
  name : "File",
  description : "File name to DMA from. If specified than the image is automatically loaded into the DMA buffer specified by ddrAddr. Can be also left empty, in this case the application is responsible to fill the DMA buffer.",
  value : "",
  type : "file",
  constraint : "",
 } ]
});

}*/
/*{

filter({
 id : "dmaOut",
 name : "Dma Out",
 description : "DMA out filter",
 image : "52",

 color : "9090b0",
 shape : "rectangle",
 group : "Input-Output Filters",

 type : "hw",
 symbol :"SIPP_DMA_ID",
 preserve : "imgSize, numPlanes, dataType",
 dataType : "UInt8, UInt16, UInt32, UInt64, Int8, Int16, Int32, half, fp16, fp32, float",
 structure :"DmaParam",
 flags : "0x00",

 sourceConstraint : "assert(false,'No output')",
 targetConstraint : "assert(target.getTargetConnections().size() == 0,'Only 1 input allowed')",

 mandatoryInputs : 1,
 inputs : [ {
       name : "",
       dataTypesOptions : ["UInt8", "UInt16", "UInt32", "UInt64", "Int8", "Int16", "Int32", "half", "fp16", "fp32","float"],
       kernelOptions : [[1,1]]
 } ],

 parameters : [  {
  id : "internal0",
  name : "ddrAddr",
  description : "Buffer address to DMA from. If defined as 'auto', the buffer will be automatically generated in DDR.",
  value : "auto",
  type : "u32",
  constraint : "",
 },{
  id : "internal1",
  name : "File",
  description : "File name to DMA into. If specified than the DMA buffer (ddrAddr) content is automatically saved into the specified file. Can be also left empty.",
  value : "output.raw",
  type : "file",
  constraint : "",
 } ],
});

}*/

#endif // __SIPP_HWDEFS_H__
