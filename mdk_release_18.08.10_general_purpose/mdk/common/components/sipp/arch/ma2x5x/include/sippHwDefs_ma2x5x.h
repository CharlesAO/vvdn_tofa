///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Config data structures for MA2x5x SIPP HW filters.
///            Most registers exposed thourgh these data structures
///            are described in detail in the MDK Programmer's Guide.
///
///            Data members marked (Private) are computed internally
///            by SIPP framework and should not be touched by user.
///

#ifndef __SIPP_HW_DEFS_MA2x5x_H__
#define __SIPP_HW_DEFS_MA2x5x_H__

//===================================================================
//Note: on SPARC, SET_REG_WORD is already defined ...
//      on Shave, it makes no sense, so only let definition for PC
#ifdef SIPP_PC
 void   SET_REG_WORD     (UInt32 addr, UInt32 value);
 UInt32 GET_REG_WORD_VAL (UInt32 addr);
 void   SET_REG_DWORD    (UInt32 addr, UInt64 value);
 UInt64 GET_REG_DWORD_VAL(UInt32 addr);
#endif

//===================================================================
//Median filter params
/// @sf_definition medianMa2x5x Median
/// @sf_description Median Filter.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8
/// @sf_type hw
/// @sf_function SIPP_MED_ID
/// @sf_inputs
/// 	- name: input0; datatypes: UInt8; kernels: 3x0, 3x3, 5x5, 7x7
 /// 	- name: input1; datatypes: UInt8, fp16; kernels: 1x0, 3x0
/// @{

/// @brief Parameter structure of the @ref median filter.
typedef struct {
    /// @sf_pfprivate yes
    UInt32  frmDim;          //see SIPP_MED_FRM_DIM_ADR  (Private)
    /// @sf_pfdesc configuration bit field, see SIPP_MED_CFG_ADR
    UInt32  cfg;             //see SIPP_MED_CFG_ADR
    /// @sf_pfdesc configuration bit field, see SIPP_MED_LUMA_ALPHA_ADR
    UInt32  lumaAlpha;       //see SIPP_MED_LUMA_ALPHA_ADR

}MedParam;
/// @}

//===================================================================
//Lens Shading Correction params
/// @sf_definition lscMa2x5x Lens Shading Correction
/// @sf_description Lens shading correction (or anti-vignetting) compensates for the effect produced by camera optics whereby the  light  intensity  of  pixels  reduces  the  further  away  from  the  centre  of  the  image  they  are.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, UInt16
/// @sf_type hw
/// @sf_function SIPP_LSC_ID
/// @sf_inputs
/// 	- datatypes: UInt8, UInt16; kernels: 1x0, 1x1
/// @{

/// @brief Parameter structure of the @ref lsc filter.
typedef struct{
	/// @sf_pfprivate yes
    UInt32  frmDim;          //see SIPP_LSC_FRM_DIM_ADR  (Private)
	/// @sf_pfprivate yes
    UInt32  fraction;        //see SIPP_LSC_FRACTION_ADR (Private)
	/// @sf_pfprivate yes
    UInt32  gmDim;           //see SIPP_LSC_GM_DIM_ADR   (Private)
	/// @sf_pfprivate yes
    UInt32  cfg;             //see SIPP_LSC_CFG_ADR      (Private)

	/// @sf_pfdesc Gain Map base
    UInt16 *gmBase;          //[Gain Map] base
	/// @sf_pfdesc Gain Map width
    UInt32  gmWidth;         //[Gain Map] width
	/// @sf_pfdesc Gain Map height
    UInt32  gmHeight;        //[Gain Map] height
	/// @sf_pfdesc data format: Planar(0), Bayer(1)
    UInt32  dataFormat;      //Planar(0), Bayer(1)
	/// @sf_pfdesc data width (8-16 bit)
    UInt32  dataWidth;       //8-16 bit
}LscParam;
/// @}

//===================================================================
//Raw block params
/// @sf_definition rawMa2x5x Raw
/// @sf_description The Raw filter performs a number of functions on raw CFA data, prior to demosaicing,  including  hot  and  cold  pixel  suppression,  Gr/Gb  imbalance  correction,  digital  gain, and statistics collection.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8, UInt16
/// @sf_type hw
/// @sf_function SIPP_RAW_ID
/// @sf_inputs
/// 	- datatypes: UInt8, UInt16; kernels: 5x5, 5x0
/// @{

/// @brief Parameter structure of the @ref raw filter.
typedef struct
{
	/// @sf_pfprivate yes
    UInt32 frmDim;           //see SIPP_RAW_FRM_DIM_ADR (Private)
	/// @sf_pfdesc Maximum local green difference reduction(see SIPP_GRGB_PLATO_ADR)
    UInt32 grgbPlat;         //see SIPP_GRGB_PLATO_ADR
	/// @sf_pfdesc Decay control of local green difference reduction(see SIPP_GRGB_SLOPE_ADR)
    UInt32 grgbDecay;        //see SIPP_GRGB_SLOPE_ADR
	/// @sf_pfdesc Filter aggressiveness control(see SIPP_BAD_PIXEL_CFG_ADR)
    UInt32 badPixCfg;        //see SIPP_BAD_PIXEL_CFG_ADR
	/// @sf_pfdesc configuration bit field(see SIPP_RAW_CFG_ADR)
    UInt32 cfg;              //see SIPP_RAW_CFG_ADR
	/// @sf_pfdesc array containing 4 gain values
    UInt32 gainSat[4];    //4 gains
	/// @sf_pfdesc maps on O_BASE(SIPP_STATS_ID)
    UInt32 *statsBase;       //maps on O_BASE(SIPP_STATS_ID)
	/// @sf_pfdesc see SIPP_RAW_STATS_PLANES_ADR
    UInt32  statsPlanes;     //see SIPP_RAW_STATS_PLANES_ADR
	/// @sf_pfprivate yes
    UInt32  statsFrmDim;     //see SIPP_STATS_FRM_DIM_ADR   (Private)
	/// @sf_pfdesc see SIPP_STATS_PATCH_CFG_ADR
    UInt32  statsPatchCfg;   //see SIPP_STATS_PATCH_CFG_ADR
	/// @sf_pfdesc see SIPP_STATS_PATCH_START_ADR
    UInt32  statsPatchStart; //see SIPP_STATS_PATCH_START_ADR
	/// @sf_pfdesc see SIPP_STATS_PATCH_SKIP_ADR
    UInt32  statsPatchSkip;  //see SIPP_STATS_PATCH_SKIP_ADR

    /// @sf_pfdesc Thresholds for AE/AWB stats - see SIPP_RAW_STATS_THRESHOLDS_ADR
    UInt32  statsThresh;
    /// @sf_pfdesc Array of coefficients for auto-focus stats filter 1
    Int32  afF1coefs[11];
    /// @sf_pfdesc Array of coefficients for auto-focus stats filter 2
    Int32  afF2coefs[11];
    /// @sf_pfdesc Minimum thresholds for per-patch auto-focus stats accumulation
    UInt32  afMinThresh;
    /// @sf_pfdesc Initial subtraction value at start of IIR filter
    UInt32  afSubtract;
    /// @sf_pfdesc Accumulation patch configuration for auto-focus statistics
    UInt32  afPatchCfg;
    /// @sf_pfdesc Start location of first (top-left) patch, and hence of the ROI, for auto-focus statistics
    UInt32  afPatchStart;

    // Not in the RAW register sets, these are oPipeBufIds
    /// @sf_pfdesc Pointer to stats Base
    UInt32 *afStatsBase;      //  O_BASE(SIPP_AF_STATS_ID)
    /// @sf_pfdesc Pointer to Luma Histogram Base
    UInt32 *histLumaBase;     //  O_BASE(SIPP_LUMA_HIST_ID)
    /// @sf_pfdesc Pointer to RGB Histogram Base
    UInt32 *histRgbBase;      //  O_BASE(SIPP_RGB_HIST_ID)

}RawParam;
/// @}

//===================================================================
//Debayer params
/// @sf_definition debayerMa2x5x Debayer
/// @sf_description This filter converts raw Bayer data into 3-channels-per-pixel RGB data.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_type hw
/// @sf_function SIPP_DBYR_ID
/// @sf_inputs
///         - datatypes: UInt8, UInt16; kernels: 11x11
/// @sf_outputbufs
///         - datatypes: UInt8, UInt16; preserve: imgSize
///         - datatypes: UInt8, fp16; preserve: imgSize, numPlains
/// @{

/// @brief Parameter structure of the @ref debayer filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;           //see SIPP_DBYR_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bit field(see SIPP_DBYR_CFG_ADR)
    UInt32 cfg;              //see SIPP_DBYR_CFG_ADR
	/// @sf_pfdesc thresholds (see SIPP_DBYR_THRES_ADR)
    UInt32 thresh;           //see SIPP_DBYR_THRES_ADR
	/// @sf_pfdesc De-worming bit field(see SIPP_DBYR_DEWORM_ADR)
    UInt32 dewormCfg;        //see SIPP_DBYR_DEWORM_ADR
    /// @sf_pfdesc De-worming bit field(see SIPP_DBYR_DEWORM_ADR)
    UInt32 lumaWeight;
}DbyrParam;
/// @}

//===================================================================
//Sharpen params
/// @sf_definition sharpenMa2x5x Sharpen
/// @sf_description The Sharpen filter enhances image sharpness. Programmable (separable, symmetric) blur filter kernel. Sharpening functionality can be disabled to use filter kernel on its own.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, fp16
/// @sf_type hw
/// @sf_function SIPP_SHARPEN_ID
/// @sf_preserve dataType, imgSize, numPlanes
/// @sf_inputs
/// 	- datatypes: UInt8, fp16; kernels: 3x3, 5x5, 7x7
/// @{

/// @brief Parameter structure of the @ref sharpen filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;           //see SIPP_SHARPEN_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bit field(see SIPP_SHARPEN_CFG_ADR)
    UInt32 cfg;              //see SIPP_SHARPEN_CFG_ADR
	/// @sf_pfdesc Sharpen  Strength(see SIPP_SHARPEN_STREN_ADR)
    UInt32 strength;         //see SIPP_SHARPEN_STREN_ADR
	/// @sf_pfdesc Mix factor for mixing constrained sharpened pixel with unconstrained sharpened pixel(see SIPP_SHARPEN_CLIP_ADR)
    UInt32 clip;             //see SIPP_SHARPEN_CLIP_ADR
	/// @sf_pfdesc Undershoot limit  control(see SIPP_SHARPEN_LIMIT_ADR)
    UInt32 limit;            //see SIPP_SHARPEN_LIMIT_ADR
	/// @sf_pfdesc Range Stop value 0 and 1(see SIPP_SHARPEN_RANGETOP_1_0_ADR)
    UInt32 rgnStop01;        //see SIPP_SHARPEN_RANGETOP_1_0_ADR
	/// @sf_pfdesc Range Stop value 2 and 3(see SIPP_SHARPEN_RANGETOP_3_2_ADR)
    UInt32 rgnStop23;        //see SIPP_SHARPEN_RANGETOP_3_2_ADR
	/// @sf_pfdesc Filter Coefficient 0 and 1(see SIPP_SHARPEN_GAUSIAN_1_0_ADR)
    UInt32 coef01;           //see SIPP_SHARPEN_GAUSIAN_1_0_ADR
	/// @sf_pfdesc Filter Coefficient 2 and 3(see SIPP_SHARPEN_GAUSIAN_1_0_ADR)
    UInt32 coef23;           //see SIPP_SHARPEN_GAUSIAN_1_0_ADR
}UsmParam;
/// @}

//===================================================================
//Luma denoise params
/// @sf_definition ydnsMa2x5x Luma Denoise
/// @sf_description This filter is designed to remove noise from a single image plane.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8, fp16
/// @sf_type hw
/// @sf_function SIPP_LUMA_ID
/// @sf_mandatoryinputs 1
/// @sf_inputs
/// 	- name: input; 		datatypes: UInt8, fp16; kernels: 11x11
/// @{

/// @brief Parameter structure of the @ref ydns filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32  frmDim;          //see SIPP_LUMA_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bit field(see SIPP_LUMA_CFG_ADR)
    UInt32  cfg;             //see SIPP_LUMA_CFG_ADR
	/// @sf_pfdesc Pointer to a 32-entry LUT(see SIPP_LUMA_LUT)
    UInt32  gaussLut[4];     //see SIPP_LUMA_LUT*_ADR
	/// @sf_pfdesc F2 4x4 2-bit LUT entries(see SIPP_LUMA_F2LUT_ADR)
    UInt32  f2;              //see SIPP_LUMA_F2LUT_ADR
    /// @sf_pfdesc LUT entries for applying Gamma to reference image
    UInt32  gammaLut[5]; //new
    /// @sf_pfdesc Pointer to (Cosine 4th law) look-up table
    UInt32  distCfg;     //addr of dist LUT
    /// @sf_pfdesc Distance-based (Cosine 4th law) look-up table X and Y tile offsets
    UInt32  distOffsets; // SIPP_LUMA_DIST_OFFSETS_ADR
    /// @sf_pfdesc Luma denoise filter full frame dimensions in pixels
    UInt32  fullFrmDim;  // SIPP_LUMA_FULL_FRM_DIM_ADR

}YDnsParam;
/// @}

//===================================================================
//Chroma denoise
/// @sf_definition chromadnsMa2x5x Chroma Denoise
/// @sf_description This filter performs chroma denoise using wide cascaded, thresholded box filters.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8
/// @sf_type hw
/// @sf_function SIPP_CHROMA_ID
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @{

/// @brief Parameter structure of the @ref chormadns filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;           //see SIPP_CHROMA_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bitfield(see SIPP_CHROMA_CFG_ADR)
    UInt32 cfg;              //see SIPP_CHROMA_CFG_ADR
	/// @sf_pfdesc thresholds(see SIPP_CHROMA_THRESH_ADR, SIPP_CHROMA_THRESH2_ADR)
    UInt32 thr[2];           //see SIPP_CHROMA_THRESH_ADR
                             //    SIPP_CHROMA_THRESH2_ADR
    /// @sf_pfdesc Sets Color to desaturate towards (see SIPP_CHROMA_GREY_POINT_ADR)
    UInt32 greyPt;
    /// @sf_pfdesc Sets coefficinets of symmetric 3x3 pre filter (see SIPP_CHROMA_CHROMA_COEFFS_ADR)
    UInt32 chrCoefs;
}ChrDnsParam;
/// @}

//===================================================================
//Lut params
/// @sf_definition lutMa2x5x Look-up table
/// @sf_description The LUT is a highly flexible lookup table which can be used for tonal curve or gamma application, or other complex functions.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8, UInt16, half, fp16
/// @sf_type hw
/// @sf_function SIPP_LUT_ID
/// @sf_inputs
/// 	- datatypes: UInt8, UInt16, half, fp16; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref lut filter.
typedef struct {
    /// @sf_pfprivate yes
    UInt32 frmDim;           //see SIPP_LUT_FRM_DIM_ADR (Private)
    /// @sf_pfdesc configuration bitfield(see SIPP_LUT_CFG_ADR)
    UInt32 cfg;              //see SIPP_LUT_CFG_ADR
    /// @sf_pfdesc see SIPP_LUT_SIZES7_0_ADR
    UInt32 sizeA;            //see SIPP_LUT_SIZES7_0_ADR
    /// @sf_pfdesc see SIPP_LUT_SIZES15_8_ADR
    UInt32 sizeB;            //see SIPP_LUT_SIZES15_8_ADR
    /// @sf_pfdesc pointer to UInt8 or fp16 Lookup Table
    void  *lut;              //ptr to UInt8 or fp16 Lookup Table
    /// @sf_pfdesc Set format of LUT
    UInt32 lutFormat;
    /// @sf_pfdesc 3x3 array of CSC matrix coeffs
    UInt16 mat[9];
    /// @sf_pfdesc Color conversion offsets in S(1,12) format
    UInt16 offset[3];

}LutParam;
/// @}

//===================================================================
//Color Combination params
/// @sf_definition colorcombMa2x5x Color Combination
/// @sf_description The Color Combination filter takes Chrominance and Luminance data that was separated from RGB previously, for the purposes of independent filtering.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, UInt16, half
/// @sf_type hw
/// @sf_function SIPP_CC_ID
/// @sf_mandatoryinputs 2
/// @sf_preserve
/// @sf_inputs
/// 	- name: luma; 		datatypes: UInt8, fp16; kernels: 1x1
///		- name: chroma; 	datatypes: UInt8;		kernels: 5x5
/// @{

/// @brief Parameter structure of the @ref colorcomb filter.
typedef struct
{
/// @sf_pfprivate yes
  UInt32 frmDim;              //see SIPP_CC_FRM_DIM_ADR (Private)
  /// @sf_pfdesc configuration bit field(see SIPP_CC_CFG_ADR)
  UInt32 cfg;                 //see SIPP_CC_CFG_ADR
  /// @sf_pfdesc coefficient array for luma+chroma  recombination(see SIPP_CC_KRGB*_ADR (4.8))
  UInt32 krgb[2];             //see SIPP_CC_KRGB*_ADR (4.8)
  /// @sf_pfdesc Color adjustment matrix(see SIPP_CC_CCM*_ADR (6.10))
  UInt32 ccm [5];             //see SIPP_CC_CCM*_ADR (6.10)
  UInt32 ccOffs;
  void *threeDLut;            //ptr to UInt8 or fp16 Lookup Table
  UInt8   lutFormat;

}ColCombParam;
/// @}

//===================================================================
//Convolution params
/// @sf_definition convolutionMa2x5x Convolution
/// @sf_description Convolution kernel is used to apply arbitrary (i.e. non-separable) convolutions.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, half
/// @sf_type hw
/// @sf_function SIPP_CONV_ID
/// @sf_inputs
/// 	- datatypes: UInt8, half; kernels: 3x3, 5x5
/// @{

/// @brief Parameter structure of the @ref convolution filter.
typedef struct
{
    /// @sf_pfprivate yes
    UInt32  frmDim;           //see SIPP_CONV_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bit field(see SIPP_CONV_CFG_ADR)
    UInt32  cfg;              //see SIPP_CONV_CFG_ADR
    /// @sf_pfdesc Convolution kernel coefficients(see SIPP_CONV_COEFF_*_ADR (fp16 values))
    UInt32  kernel[15];       //see SIPP_CONV_COEFF_*_ADR (fp16 values)
    /// @sf_pfdesc Convolution kernel coefficients(see SIPP_CONV_COEFF_*_ADR (fp16 values))
    UInt32  shadowKernel[15];       //see SIPP_CONV_COEFF_*_ADR (fp16 values)
}ConvParam;
/// @}

//===================================================================
//Harris corner detect params
/// @sf_definition harriscornersMa2x5x Harris Corner Detector
/// @sf_description The Harris corners filter performs corner detection on U8F image data.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype half,fp16,fp32,float
/// @sf_type hw
/// @sf_preserve numPlanes, imgSize
/// @sf_function SIPP_HARRIS_ID
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5, 7x7, 9x9
/// @{

/// @brief Parameter structure of the @ref harriscorners filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;             //see SIPP_HARRIS_FRM_DIM_ADR (Private)
    /// @sf_pfdesc configuration bit field(see SIPP_HARRIS_CFG_ADR)
	UInt32 cfg;                //see SIPP_HARRIS_CFG_ADR
	/// @sf_pfdesc value that changes the response of the edges(FP32)(see SIPP_HARRIS_K_ADR)
    float  kValue;             //see SIPP_HARRIS_K_ADR
}HarrisParam;
/// @}

//===================================================================
//Polyphase Scaler params
/// @sf_definition polyphasefirMa2x5x Polyphase FIR Scaler
/// @sf_description The poly-phase FIR filter scaler is suitable for high-quality implementations of scaling using e.g. Lanczos resampling.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, half
/// @sf_type hw
/// @sf_function SIPP_UPFIRDN_ID
/// @sf_flags SIPP_RESIZE
/// @sf_preserve
/// @sf_inputs
/// 	- datatypes: UInt8, half; kernels: 3x3, 5x5, 7x7
/// @{


typedef enum {                 // unimplemented !
    POLY_MODE_AUTO    = 0,     // unimplemented !
    POLY_MODE_ADVANCE = 1      // unimplemented !
}PolyModes;                    // unimplemented !
                               // unimplemented !
typedef enum {                 // unimplemented !
    POLY_LANCZOS    = 0,       // unimplemented !
    POLY_BICUBIC    = 1,       // unimplemented !
    POLY_BILINEAR   = 2        // unimplemented !
}PolyScalerType;               // unimplemented !

typedef enum {
    POLY_PLANE_ALL  = 0,
    POLY_PLANE_Y    = 1,
    POLY_PLANE_U    = 2,
    POLY_PLANE_V    = 3,
    POLY_PLANE_UV   = 4,
}PolyPlaneMode;

/// @brief Parameter structure of the @ref polyphasefir filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32         cfgReg;     //(Private)
	/// @sf_pfprivate yes
    UInt32         kerSz;      //(Private)
	/// @sf_pfprivate yes
    UInt32         frmDimPar;  //see SIPP_UPFIRDN_FRM_IN_DIM_ADR  (Private)
	/// @sf_pfprivate yes
    UInt32         frmDimFlt;  //see SIPP_UPFIRDN_FRM_OUT_DIM_ADR (Private)

    /// @sf_pfprivate yes
    PolyModes      mode;       //unimplemented !
    /// @sf_pfprivate yes
    PolyScalerType autoType;   //unimplemented !

  //These parameters should be set just for advance mode
  //for auto mode will be calculated internally by sipp model
  	/// @sf_pfdesc clamp enable
    UInt32 clamp;             // : 1;
	/// @sf_pfdesc Horizontal Denominator factor
    UInt32 horzD;             // : 6; Horizontal Denominator factor
	/// @sf_pfdescHorizontal Numerator factor
    UInt32 horzN;             // : 5; Horizontal Numerator   factor
	/// @sf_pfdesc Vertical Denominator factor
    UInt32 vertD;             // : 6; Vertical   Denominator factor
	/// @sf_pfdesc Vertical Numerator factor
    UInt32 vertN;             // : 5; Vertical   Numerator   factor
	/// @sf_pfdesc pointer to horizontal filter coefficients(see SIPP_UPFIRDN_HCOEFF_*_ADR)
    UInt8 *horzCoefs;        //see SIPP_UPFIRDN_HCOEFF_*_ADR
	/// @sf_pfdesc pointer to vertical filter coefficients(see SIPP_UPFIRDN_VCOEFF_*_ADR)
    UInt8 *vertCoefs;        //see SIPP_UPFIRDN_VCOEFF_*_ADR
    /// @sf_pfdesc Enables override of filter plane mode for runnig multiple units on same stream
    PolyPlaneMode  planeMode;

}PolyFirParam;
/// @}

//===================================================================
//Edge operator params
/// @sf_definition edgeoperatorMa2x5x Edge operator
/// @sf_description Flexible 3x3 edge-detection operator suitable for implementation of e.g. Sobel filter.
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_preserve imgSize, numPlains
/// @sf_outdatatype UInt8, UInt16
/// @sf_type hw
/// @sf_function SIPP_EDGE_OP_ID
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @{

/// @brief Parameter structure of the @ref edgeoperator filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;            //see SIPP_EDGE_OP_FRM_DIM_ADR (Private)
	/// @sf_pfdesc configuration bitfield(see SIPP_EDGE_OP_CFG_ADR)
    UInt32 cfg;               //see SIPP_EDGE_OP_CFG_ADR
	 /// @sf_pfdesc Edge operator X coefficients(see SIPP_EDGE_OP_XCOEFF_ADR)
    UInt32 xCoeff;            //see SIPP_EDGE_OP_XCOEFF_ADR
	/// @sf_pfdesc Edge operator Y coefficients(see SIPP_EDGE_OP_YCOEFF_ADR)
    UInt32 yCoeff;            //see SIPP_EDGE_OP_YCOEFF_ADR
}EdgeParam;
/// @}

//===================================================================
//Sigma params
/// @sf_definition sigmaDnsMa2x5x Sigma Denoise
/// @sf_description Sigma Denoise & Black level correction Filter
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, UInt16
/// @sf_type hw
/// @sf_function SIPP_SIGMA_ID
/// @sf_inputs
///     - datatypes: UInt8, UInt16; kernels: 5x5
/// @{

/// @brief Parameter structure of the @ref edgeoperator filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;            //see SIPP_EDGE_OP_FRM_DIM_ADR (Private)
    /// @sf_pfdesc ????
    UInt32 thresh[2];         //see SIPP_EDGE_OP_CFG_ADR
    /// @sf_pfdesc configuration bitfield(see SIPP_EDGE_OP_CFG_ADR)
    UInt32 cfg;               //see SIPP_EDGE_OP_CFG_ADR

    /// @sf_pfdesc Bayer Pattern
    UInt32 bayerPattern;
    /// @sf_pfdesc Black level Correction GR
    UInt32 blcGR;
    /// @sf_pfdesc Black level Correction R
    UInt32 blcR;
    /// @sf_pfdesc Black level Correction B
    UInt32 blcB;
    /// @sf_pfdesc Black level Correction GB
    UInt32 blcGB;
}SigmaParam;
/// @}

//===================================================================
//Generate Chroma params
/// @sf_definition chromaGenMa2x5x Chroma Generation
/// @sf_description Spatial sub-sampling, Purple Flare reduction, Desaturation & Chroma Generation Filter
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8
/// @sf_type hw
/// @sf_function SIPP_CGEN_ID
/// @sf_preserve numPlanes
/// @sf_flags SIPP_RESIZE
/// @sf_inputs
///     - datatypes: UInt8, UInt16; kernels: 3x3, 6x6
/// @{

/// @brief Parameter structure of the @ref edgeoperator filter.
typedef struct {
/// @sf_pfprivate yes
    UInt32 frmDim;
    /// @sf_pfdesc configuration bitfield
    UInt32 cfg;
    /// @sf_pfdesc ???
    UInt32 yCoefs;
    /// @sf_pfdesc ???
    UInt32 chrCoefs;
}GenChrParam;
/// @}

//===================================================================
//DoGLtm params
/// @sf_definition dogLtmMa2x5x DoG LTM
/// @sf_description Spatial Local Tone Mapping plus Noise reduction based on a Difference of Gaussians
/// @sf_group SIPP_MA2x5x_Hardware_Filters
/// @sf_myriadtarget MA2x5x
/// @sf_outdatatype UInt8, fp16
/// @sf_type hw
/// @sf_function SIPP_DOGL_ID
/// @sf_inputs
///     - datatypes: UInt8, fp16; kernels: 3x3, 5x5, 7x7, 9x9, 11x11, 13x13, 15x15
/// @{

typedef struct
{
/// @sf_pfprivate yes
    UInt32   frmDim;
    /// @sf_pfdesc configuration bitfield
    UInt32   cfg;
    UInt8 *  dogCoeffs11;  // [6]
    UInt8 *  dogCoeffs15;  // [8]
    UInt8    dogStrength;
    UInt16 * ltmCurves; // [16*8];     /* U12f values */

} DogLtmParam;
/// @}

//===================================================================
//Mipi-RX  params
/// @defgroup mipirx MIPI Rx
/// @brief Flexible streaming processing of input directly from MIPI Rx including windowing, sub-sampling, data selection, black level subtraction (for RAW input) and data format conversion.
/// @ingroup SIPP_Input-Output_Filters
/// @par Output data type(s):\n
/// UInt8, UInt16, UInt32, half, fp16
/// @par Preserve:\n
/// @par Filter type:\n
///  hw
/// @par   Filter functions:\n
///     SIPP_MIPI_RX0_ID, SIPP_MIPI_RX1_ID, SIPP_MIPI_RX2_ID, SIPP_MIPI_RX3_ID
/// @par Inputs:\n
/// @par Madatory inputs:\n
/// 0
/// @{

/// @brief Parameter structure of the @ref mipirx filter.
typedef struct
{
    /// @par Private:\n
    /// yes
    UInt32 frmDim;            //see SIPP_MIPI_RX*_FRM_DIM_ADR (Private)
    /// @brief configuration bit field(see SIPP_MIPI_RX*_CFG_ADR)
    UInt32 cfg;               //see SIPP_MIPI_RX*_CFG_ADR
    /// @brief Window x co-ordinate configuration
    UInt32 winX[4];           //see SIPP_MIPI_RX*_X*_ADR
    /// @brief Window y co-ordinate configuration
    UInt32 winY[4];           //see SIPP_MIPI_RX*_Y*_ADR
    /// @brief Selection enables and least significant bit for windows 0 and 1
    UInt32 sel01;             //see SIPP_MIPI_RX*_SEL01_ADR
    /// @brief Selection enables and least significant bit for windows 2 and 3
    UInt32 sel23;             //see SIPP_MIPI_RX*_SEL23_ADR
    /// @brief Selection mask
    UInt32 selMask[4];        //see SIPP_MIPI_RX*_MASK*_ADR
    /// @brief Black levels for windows 0 and 1 or even lines of RAW Bayer data
    UInt32 black01;           //see SIPP_MIPI_RX*_BLACK01_ADR
    /// @brief Black levels for windows 2 and 3 or odd lines of RAW Bayer data
    UInt32 black23;           //see SIPP_MIPI_RX*_BLACK23_ADR
    /// @brief Vertical back porch and private slice stride
    UInt32 vbp;               //see SIPP_MIPI_RX*_VBP_ADR

} MipiRxParam;
/// @}

//===================================================================
//Mipi-TX  params
/// @defgroup mipitx MIPI Tx
/// @brief Timing generation for MIPI Tx controller parallel interface for CSI-2/DSI output.
/// @ingroup SIPP_Input-Output_Filters
///  @par Output data type(s):\n
///@par Filter type:\n
///  hw
/// @par   Filter functions:\n
///  SIPP_MIPI_TX0_ID, SIPP_MIPI_TX1_ID
/// @par Inputs:\n
///     - datatypes: UInt8, UInt16, UInt32, half, fp16; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref mipirx filter.
typedef struct
{
    /// @par Private:\n
    /// yes
    UInt32 frmDim;             //see SIPP_MIPI_TX*_FRM_DIM_ADR (Private)
    /// @brief configuration bit field
    UInt32 cfg;                //see SIPP_MIPI_TX*_CFG_ADR

    //UInt32 lineCount;//RO
    /// @brief Line count at which to generate line compare interrupt

    UInt32 lineCompare;        //see SIPP_MIPI_TX*_LINE_COMP_ADR
    //UInt32 vStatus;  //RO
    /// @brief Vertical interval in which to generate vertical interval interrupt
    UInt32 vCompare;           //see SIPP_MIPI_TX*_VCOMP_ADR

    //Horizontal timings
    /// @brief Specifies the width, in PCLK clock periods, of the horizontal sync pulse (value programmed is HSW-1)
    UInt32 hSyncWidth;         //see SIPP_MIPI_TX*_HSYNC_WIDTH_ADR
    /// @brief Specifies the number of PCLK clocks from the end of the horizontal sync pulse to the start of horizontal active (value programmed is HBP so a back porch of 0 cycles can be set)
    UInt32 hBackPorch;         //see SIPP_MIPI_TX*_H_BACKPORCH_ADR
    /// @brief Specifies the number of PCLK clocks in the horizontal active section (value programmed is AVW-1)
    UInt32 hActiveWidth;       //see SIPP_MIPI_TX*_H_ACTIVEWIDTH_ADR
    /// @brief Specifies the number of PCLK clocks from end of active video to the start of horizontal sync (value programmed is HFP)
    UInt32 hFrontPorch;        //see SIPP_MIPI_TX*_H_FRONTPORCH_ADR

    // Vertical timings (there are 2 sets for interlaced,
    // but for now deal with progressive case)
    /// @brief Specifies the width in lines of the vertical sync pulse (value programmed is VSW-1).
    UInt32 vSyncWidth;         //see SIPP_MIPI_TX*_VSYNC_WIDTH_ADR
    /// @brief Specifies the number of lines from the end of the vertical sync pulse to the start of vertical active (value programmed is VBP)
    UInt32 vBackPorch;         //see SIPP_MIPI_TX*_V_BACKPORCH_ADR
    /// @brief Specifies the number of lines in the vertical active section (value programmed is AVH-1)
    UInt32 vActiveHeight;      //see SIPP_MIPI_TX*_V_ACTIVEHEIGHT_ADR
    /// @brief Specifies the number of lines from the end of active data to the start of vertical sync pulse (value programmed is VFP).
    UInt32 vFrontPorch;        //see SIPP_MIPI_TX*_V_FRONTPORCH_ADR
    /// @brief Number of PCLKs from the start of the last horizontal sync pulse in the Vertical Front Porch to the start of the vertical sync pulse.
    UInt32 vSyncStartOff;      //see SIPP_MIPI_TX*_VSYNC_START_OFFSET_ADR
    /// @brief Number of PCLKs from the end of the last horizontal sync pulse in the Vertical Sync Active to the end of the vertical sync pulse
    UInt32 vSyncEndOff;        //see SIPP_MIPI_TX*_VSYNC_END_OFFSET_ADR

} MipiTxParam;
/// @}

//===================================================================
//Mipi-TX loopback debug params
typedef struct
{
    UInt32 txID;               //SIPP_MIPI_TX0_ID or SIPP_MIPI_TX1_ID
    UInt8* imgAddr;            //full image base address
    UInt32 imgW;               //full image width
    UInt32 imgH;               //full image height
    UInt32 bpp;                //bytes per pixel
    UInt32 hbp;                //timing: horizontal back porch [pclk]
    UInt32 hfp;                //timing: horizontal front porch [pclk]
    UInt32 hsync;              //timing: horizontal sync [pclk]
    UInt32 vsync;              //timing: vertical sync [lines]

} MipiTxLoopbackParam;

//===================================================================
//Mipi-RX loopback debug params
typedef struct
{
    UInt32 rxID;               //SIPP_MIPI_RX1_ID or SIPP_MIPI_RX3_ID
    UInt8* imgAddr;            //full image base address
    UInt32 imgW;               //full image width
    UInt32 imgH;               //full image height
    UInt32 bpp;                //bytes per pixel              //bytes per pixel

} MipiRxLoopbackParam;

//===================================================================
//Utils that rely on structures defined above
void packConv5x5CCM     (ConvParam    *cfg, UInt16 *ccm5x5);
void packConv3x3CCM     (ConvParam    *cfg, UInt16 *ccm3x3);
void packColCombCCM     (ColCombParam *cfg, float  *ccm3x3);
void packLumaDnsGaussLut(YDnsParam    *cfg, UInt8  *lut   );

void cfgMipiRxLoopback  (MipiRxLoopbackParam *cfg);
void cfgMipiTxLoopback  (MipiTxLoopbackParam *cfg);
void startMipiTxLoopback(MipiTxLoopbackParam *cfg);

#endif // !__SIPP_HW_DEFS_MA2x5x_H__
