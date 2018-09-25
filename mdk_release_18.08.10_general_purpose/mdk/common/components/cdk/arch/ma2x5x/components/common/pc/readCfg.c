//PC only utils that scan arguments from text file
#if !defined(__sparc)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sipp.h> //for fp16 support

#include <common/leon/ipipe.h>
#include <common/leon/ipipeSipp.h>
#include <common/leon/ipipeUtils.h>

#define INI_TYPE_INT		1
#define INI_TYPE_STRING		2
#define INI_TYPE_FLOAT		3

#ifdef _WIN32
#define STRDUP	_strdup
#define MATCH(a, b)    (!_stricmp((a), (b)))
#else
#define STRDUP	strdup
#define MATCH(a, b)    (!strcasecmp((a), (b)))
#endif

typedef int (*ini_callback_t)(const char *section, const char *name, const void *val, int size, int type);

//#include "common.h"

static ispCfg *iCfg; //global ref to current config ISP data struct
static char *section_name;
static int lno;

//##########################################################################
/* Eat leading whitespace */
static char * eat_ws(char *c)
{
    int i, l = strlen(c);

    for (i = 0; i < l; i++) {
        if (*c != '\t' && *c != ' ') {
            break;
        }
        c++;
    }

    return c;
}

//##########################################################################
static int parse_section_name(char *c)
{
    c++;	/* Skip '[' */
    c = eat_ws(c);

    if (section_name != NULL) {
        free(section_name);
    }
    section_name = STRDUP(c);
    if (section_name == NULL) {
        perror("strdup");
        abort();
    }
    c = section_name;
    while (*c != ']' && *c != '\0') {
        c++;
    }

    if (c == '\0') {
        return -1;
    }

    while ((*c == ' ' || *c == '\t') && c != section_name) {
        c--;
    }
    *c = '\0';

    return 0;
}

//##########################################################################
/* Supress trailing whitespace */
static void clobber_ws_tr(char *ins)
{
    char	*c;

    c = ins + strlen(ins) - 1;
    while (c != ins) {
        if (*c == '\n' || *c == '\r' || *c == ' ' || *c == '\t') {
            *c = '\0';
            c--;
        } else {
            break;
        }
    }
}

//##########################################################################
static int cb(ini_callback_t callback, char *section, char *name, void *val, int size, int type)
{
    clobber_ws_tr(section);
    clobber_ws_tr(name);
    if (type == INI_TYPE_STRING) {
        clobber_ws_tr((char *)val);
    }

    return callback(section, name, val, size, type);
}

//##########################################################################
static int parse_scalar(char *section, char *name, char *val, ini_callback_t callback)
{
    char	*c, *r;
    float	f;
    int	i;

    if (isdigit(*val) || *val == '-' || *val == '.') {
        c = val;
        while (*c != '\0' && (isdigit(*c) || *c == '-')) {
            c++;
        }
        if (*c == '.') {
            f = strtod(val, &r);
            if (r == val) {
                fprintf(stderr,
                    "Couldn't parse floating point value\n");
                return -1;
            }
            cb(callback, section, name, &f, 1, INI_TYPE_FLOAT);
        } else {
            if (val[0] == '0' && val[1] == 'x') {
                i = strtoul(val, &r, 0);
            } else {
                i = strtol(val, &r, 0);
            }
            if (r == val) {
                fprintf(stderr,
                    "Couldn't parse integer value\n");
                return -1;
            }
            cb(callback, section, name, &i, 1, INI_TYPE_INT);
        }
    } else {
        cb(callback, section, name, val, strlen(val), INI_TYPE_STRING);
    }

    return 0;
}

//##########################################################################
static int parse_array(FILE *fi, char *section_name, char *name, ini_callback_t callback)
{
    char	*c, *c2;
    int	arraysize = 0, i, *iptr = NULL;
    int	isfloat = 0, rc;
    char	buf[1024];
    char	*r;
    float	f, *fptr = NULL;
    long	fpos;

    while (fpos = ftell(fi), fgets(buf, sizeof (buf), fi) != NULL) {
        lno++;
        buf[sizeof (buf) - 1] = '\0';
        c = buf;
        c = eat_ws(c);
        if (*c == ';') {
            /* Skip comment line */
            continue;
        }
        if (!isdigit(*c) && *c != '.' && *c != '-') {
            /*
            * This line is not a continuation of the array...
            * undo last fgets.
            */
            fseek(fi, fpos, SEEK_SET);
            lno--;
            break;
        }
        /*
        * An array must be either all floats, or all ints.  For an
        * array of floats, all values *must* contain a decimal point.
        * Look at the first value to determine if it's an array
        * of floats or ints.
        */
        if (arraysize == 0) {
            c2 = c;
            while (isdigit(*c2) || *c2 == '-') {
                c2++;
            }
            if (*c2 == '.') {
                isfloat = 1;
            }
        }

        while (1) {
            c = eat_ws(c);
            if (!isdigit(*c) && *c != '.') {
                if (*c == '\0' || *c == '\r' || *c == '\n') {
                    break;
                }
            }

            if (isfloat) {
                f = strtod(c, &r);
                if (r == c) {
                    fprintf(stderr,
                        "Couldn't parse floating point value\n");
                    return -1;
                }
                fptr = (float *)realloc(fptr,
                    (arraysize+1) * sizeof (float));
                if (fptr == NULL) {
                    perror("realloc");
                    abort();
                }
                fptr[arraysize] = f;
            } else {
                if (c[0] == '0' && c[1] == 'x') {
                    i = strtoul(c, &r, 0);
                } else {
                    i = strtol(c, &r, 0);
                }
                if (r == c) {
                    fprintf(stderr,
                        "Couldn't parse integer value\n");
                    return -1;
                }
                iptr = (int *)realloc(iptr,
                    (arraysize+1) * sizeof (int));
                if (iptr == NULL) {
                    perror("realloc");
                    abort();
                }
                iptr[arraysize] = i;
            }

            c = r;

            arraysize++;
        }
    }

    if (arraysize == 0) {
        fprintf(stderr, "Expecting array values\n");
        return -1;
    }

    if (isfloat) {
        rc = cb(callback, section_name, name, fptr, arraysize, INI_TYPE_FLOAT);
        free(fptr);
    } else {
        rc = cb(callback, section_name, name, iptr, arraysize, INI_TYPE_INT);
        free(iptr);
    }

    return rc;
}

//##########################################################################
static int parse_value(FILE *f, char *c, ini_callback_t callback)
{
    char	*c2, *name = c;

    while (*c != '\n' && *c != '\r' && *c != '\0' && *c != '=') {
        c++;
    }

    if (*c != '=') {
        fprintf(stderr, "Expected \"=\" in value assignment\n");
        return -1;
    }

    c2 = c-1;
    while ((*c2 == '\t' || *c2 == ' ') && c2 != name) {
        c2--;
    }
    c2[1] = '\0';

    /* Scalar value (on same line) or array (on following line(s))? */
    c2 = c = eat_ws(c+1);
    while (*c != '\0' && *c != '\n' && *c != '\r') {
        if (*c == '.' || *c == '_' || isalnum(*c)) {
            return parse_scalar(section_name, name, c2, callback);
        }
        c++;
    }

    return parse_array(f, section_name, name, callback);
}

//##########################################################################
static int parse_line(FILE *f, char *c, ini_callback_t callback)
{
    int	i, l = strlen(c);

    for (i = 0; i < l; i++) {
        if (c[i] == ';') {
            c[i] = '\0';	/* Remove comment */
        }
    }

    c = eat_ws(c);

    if (*c == '\n' || *c == '\r' || *c == '\0') {
        /* Comment or blank line */
        return 0;
    }

    if (*c == '[') {
        if (parse_section_name(c) == -1) {
            return -1;
        }
        return 0;
    }

    if (section_name == NULL) {
        fprintf(stderr, "Expecting section name\n");
        return -1;
    }

    if (parse_value(f, c, callback) == -1) {
        return -1;
    }

    return 0;
}

//##########################################################################
//we read a Hr(Human Readable) table form text file, but then convert
//to fp16 SIPP-ready-to-use under the hood (OS Leon will do this on chip)
//
float HrLut[8192]; //human readable lut table (read from config.txt)
int   HrLutSize;
int   HrLutTargetSize; 

static half lutFormula(half input)
{
    // Apply Gamma
    if (input > 1.0f) {
        return 1.0f;
    } else {
        return HrLut[(int)(input * (HrLutSize-1))];
    }
}

//##########################################################################
// Local tone mapping filter used in STILL pipeline
// - src is 15x9 float
// - dst is 16x10 fp16 (last row and column 

float HrLtmCurves[15*9]; //global copy to hold human readable data

void convertLtmCurves(float *src, uint16_t *dest)
{
    int i, j;

    // Local Tone Mapping.  Curves are a 9x15 2D array, but the filter expects
    // the last line and last row to be replicated (so it expects a 10x16 array)
    for (i = 0; i < 15; i++)
    {
        for (j = 0; j < 9; j++)
        {
            dest[i*10+j] = f32_to_f16(src[i*9+j]);
        }
    }
    // Replicate last row
    for (i = 0; i < 9; i++)
    {
        dest[10*15+i] = dest[10*14+i];
    }
    // Replicate last column
    for (i = 0; i < 16; i++)
    {
        dest[i*10 + 9] = dest[i*10 + 8];
    }
}

//##########################################################################
#if defined(_WIN32)
//stupid visualC++ 2010 doesn't support C99
float log2f(float data)
{
    return logf(data)/logf(2);
}
#endif

//##########################################################################
static void
    luma_denoise_ref_gen_lut_dist(icIspConfig *conf,
    float angle_of_view, int image_width, int image_height)
{
    float   angle;
    int     w2 = image_width/2;
    int     h2 = image_height/2;
    int     maxval, shift, dx2_plus_dy2;
    int     i;
    float   cornerval;
    uint8_t *lut = conf->lumaDenoiseRef.lutDist;

    /* Get half the angle, in radians */
    angle = (angle_of_view/2) / 360 * 2*M_PI;

    /* Maximum value that can be put into LUT */
    maxval = w2*w2 + h2*h2;

    /*
    * Find how many bits to shift values right by, so that we can use
    * an 8-bit LUT.
    */
    shift = floorf(log2f(maxval)) + 1 - 8;
    conf->lumaDenoiseRef.shift = shift; //alu
    cornerval = sqrtf(w2*w2+h2*h2);

    /*
    * LUT is indexed at runtime by (dx^2+dy^2) >> shift, where dx, dy
    * is the x/y displacement of the current pixel from the image centre.
    */
    for (i = 0; i < 255; i++) {
        dx2_plus_dy2 = i << shift;
        lut[i] = powf(cosf(sqrtf(dx2_plus_dy2) / cornerval * angle), 4) * 255 + .5;
    }
}

//##########################################################################
static void
    luma_denoise_ref_gen_lut_gamma(icIspConfig *conf, float gamma)
{
    uint8_t *lut = conf->lumaDenoiseRef.lutGamma;
    int     i;

    for (i = 0; i < 256; i++) {
        lut[i] = powf((float)i / 255, gamma) * 255 + .5;
    }
}

//##########################################################################
static int callback(const char *section, const char *name, const void *val, int size, int type)
{
    int    error = 0, i;
    icIspConfig *cfg = iCfg->ispParams;

    switch (type) {
    case INI_TYPE_INT:
        i = *(int *)val;
        if (MATCH(section, "general")) {
            if (MATCH(name, "width")) { //TBD: cleanup; only BAYER configs are like this for video, 
                iCfg->iSize.w = i; 
                iCfg->oSize.w = i - 10; //TBD: fix
            } else if (MATCH(name, "height")) {
                iCfg->iSize.h = i;
                iCfg->oSize.h = i - 10;//TBD: fix
            } else if (MATCH(name, "bits")) {
                cfg->pipelineBits = i;
            } else if (MATCH(name, "skip_lines")) {
                iCfg->skipLines = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "lsc")) {
            if (MATCH(name, "mesh_width")) {
                cfg->lsc.lscWidth = i;
            } else if (MATCH(name, "mesh_height")) {
                cfg->lsc.lscHeight = i;
            } else if (MATCH(name, "mesh")) {
                if ((cfg->lsc.pLscTable = (uint16_t *)malloc(size *
                    sizeof (uint16_t))) == NULL) {
                        perror("malloc");
                        abort();
                }
                for (i = 0; i < size; i++) {
                    cfg->lsc.pLscTable[i] = ((int *)val)[i];
                }
            } else {    
                error = 1;
            }


        } else if (MATCH(section, "AWB Stats")) {
            if (MATCH(name, "first_patch_x")) {
                cfg->aeAwbConfig.firstPatchX = i;
            } else if (MATCH(name, "first_patch_y")) {
                cfg->aeAwbConfig.firstPatchY = i;
            } else if (MATCH(name, "patch_width")) {
                cfg->aeAwbConfig.patchWidth = i;
            } else if (MATCH(name, "patch_height")) {
                cfg->aeAwbConfig.patchHeight = i;
            } else if (MATCH(name, "patch_gap_x")) {
                cfg->aeAwbConfig.patchGapX = i;
            } else if (MATCH(name, "patch_gap_y")) {
                cfg->aeAwbConfig.patchGapY = i;
            } else if (MATCH(name, "n_patches_x")) {
                cfg->aeAwbConfig.nPatchesX = i;
            } else if (MATCH(name, "n_patches_y")) {
                cfg->aeAwbConfig.nPatchesY = i;
            } else if (MATCH(name, "sat_thresh")) {
                cfg->aeAwbConfig.satThresh = i;
            } else {
                error = 1;
            }

        } else if (MATCH(section, "wbGains")) {
            if (MATCH(name, "gain_r")) {
                cfg->wbGains.gainR = i;
            } else if (MATCH(name, "gain_g")) {
                cfg->wbGains.gainG = i;
            } else if (MATCH(name, "gain_b")) {
                cfg->wbGains.gainB = i;
            } else {
                error = 1;
            }

        } else if (MATCH(section, "raw")) {
            if (MATCH(name, "output_bits")) {
                cfg->raw.outputBits = i;
            }
            else if (MATCH(name, "gain_gr")) {
                cfg->raw.gainGr = i;
            } else if (MATCH(name, "gain_gb")) {
                cfg->raw.gainGb = i;
            } else if (MATCH(name, "gain_r")) {
                cfg->raw.gainR = i;
            } else if (MATCH(name, "gain_b")) {
                cfg->raw.gainB = i;
            } else if (MATCH(name, "sat_gr")) {
                cfg->raw.clampGr = i;
            } else if (MATCH(name, "sat_gb")) {
                cfg->raw.clampGb = i;
            } else if (MATCH(name, "sat_r")) {
                cfg->raw.clampR = i;
            } else if (MATCH(name, "sat_b")) {
                cfg->raw.clampB = i;
            } else if (MATCH(name, "dpc_alpha_g_hot")) {
                cfg->raw.dpcAlphaHotG = i;
            } else if (MATCH(name, "dpc_alpha_rb_hot")) {
                cfg->raw.dpcAlphaHotRb = i;
            } else if (MATCH(name, "dpc_alpha_g_cold")) {
                cfg->raw.dpcAlphaColdG = i;
            } else if (MATCH(name, "dpc_alpha_rb_cold")) {
                cfg->raw.dpcAlphaColdRb = i;
            } else if (MATCH(name, "dpc_noise_level")) {
                cfg->raw.dpcNoiseLevel = i;
            } else if (MATCH(name, "grgb_imbal_plat_dark")) {
                cfg->raw.grgbImbalPlatDark = i;
            } else if (MATCH(name, "grgb_imbal_decay_dark")) {
                cfg->raw.grgbImbalDecayDark = i;
            } else if (MATCH(name, "grgb_imbal_plat_bright")) {
                cfg->raw.grgbImbalPlatBright = i;
            } else if (MATCH(name, "grgb_imbal_decay_bright")) {
                cfg->raw.grgbImbalDecayBright = i;
            } else if (MATCH(name, "grgb_imbal_threshold")) {
                cfg->raw.grgbImbalThr = i;
            } else {    
                error = 1;
            }
        } else if (MATCH(section, "demosaic")) {
            if (MATCH(name, "mix_slope")) {
                cfg->demosaic.dewormSlope = i;
            } else if (MATCH(name, "mix_offset")) {
                cfg->demosaic.dewormOffset = i;
            } else if (MATCH(name, "mix_gradient_mul")) {
                cfg->demosaic.dewormGradientMul = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "median")) {
            if (MATCH(name, "kernel_size")) {
                cfg->median.kernelSize = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "Luma Denoise")) {
            if (MATCH(name, "alpha")) {
                cfg->lumaDenoise.alpha = i;
            } else if (MATCH(name, "f2")) {
                cfg->lumaDenoise.f2 = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "Purple Flare")) {
            if (MATCH(name, "strength")) {
                cfg->purpleFlare.strength = i;
            } else {
                error = 1;
            }
        } 
        else if (MATCH(section, "Chroma Denoise")) {
            if (MATCH(name, "th_r")) {
                cfg->chromaDenoise.th_r = i;
            } else if (MATCH(name, "th_g")) {
                cfg->chromaDenoise.th_g = i;
            } else if (MATCH(name, "th_b")) {
                cfg->chromaDenoise.th_b = i;
            } else if (MATCH(name, "limit")) {
                cfg->chromaDenoise.limit = i;
            } else if (MATCH(name, "h_enab")) {
                cfg->chromaDenoise.hEnab = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "Filter Ltm")) {
            if (MATCH(name, "th1")) {
                cfg->ltmLBFilter.th1 = i;
            } else if (MATCH(name, "th2")) {
                cfg->ltmLBFilter.th2 = i;
            } else if (MATCH(name, "limit")) {
                cfg->ltmLBFilter.limit = i;
            } else {
                error = 1;
            }
        } else if (MATCH(section, "Grey Desaturate")) {
            if (MATCH(name, "offset")) {
                cfg->greyDesat.offset = i;
            } else if (MATCH(name, "slope")) {
                cfg->greyDesat.slope = i;
            } else if (MATCH(name, "grey_cr")) {
                cfg->greyDesat.grey_cr = i;
            } else if (MATCH(name, "grey_cg")) {
                cfg->greyDesat.grey_cg = i;
            } else if (MATCH(name, "grey_cb")) {
                cfg->greyDesat.grey_cb = i;
            } else {
                error = 1;
            }

        } else if (MATCH(section, "Gamma")) {

            if (MATCH(name, "target_size")) {
                HrLutTargetSize = i;
                cfg->gamma.size = HrLutTargetSize;
            }
        }
        else {
            error = 1;
        }
        break;



    case INI_TYPE_FLOAT: {
        float    f = *(float *)val;
        fp16    f16(f);
        int    i = f16.getPackedValue();

            if (MATCH(section, "Mipi RX")) 
            {
                if (MATCH(name, "black_level_r"))
                {
                    /// for data in range [0..1]
                    cfg->blc.r = (unsigned int) (f * 1023.0f);
                }
                else 
                    if (MATCH(name, "black_level_b"))
                    {
                    /// for data in range [0..1]
                        cfg->blc.b = (unsigned int) (f * 1023.0f);
                    }
                else 
                    if (MATCH(name, "black_level_gb"))
                    {
                    /// for data in range [0..1]
                        cfg->blc.gb = (unsigned int) (f * 1023.0f);
                    }
                else 
                    if (MATCH(name, "black_level_gr"))
                    {
                    /// for data in range [0..1]
                        cfg->blc.gr = (unsigned int) (f * 1023.0f);
                    }
                else 
                {
                    error = 1;
                }
            }

#if 0
        else if (MATCH(section, "Chroma Gen")) {
            if (MATCH(name, "epsilon")) {
                cfg->chromaGen.epsilon = f;
            } else {
                error = 1;
            }
        }
#else
        else if (MATCH(section, "Chroma Gen")) {
            if (MATCH(name, "epsilon")) {
                cfg->chromaGen.epsilon = f;
            } else if (MATCH(name, "scale")) { 
                if (size != 3) {
                    fprintf(stderr, "\"scale\" should be a 3-value array)\n");
                    error = 1;
                } else {
                    //for (i = 0; i < 3; i++) {
                    //    cfg->chromaGen.scale[i] = ((float *)val)[i];
                    //}
                    cfg->chromaGen.scaleR = ((float *)val)[0];
                    cfg->chromaGen.scaleG = ((float *)val)[1];
                    cfg->chromaGen.scaleB = ((float *)val)[2];
                }
            } else {
                error = 1;
            }
        }
#endif

        /*
        else if (MATCH(section, "Purple Flare")) {
        if (MATCH(name, "strength")) {
        cfg->purpleFlare.strength = f;
        } else {
        error = 1;
        }
        } 
        */
        else if (MATCH(section, "Luma Denoise")) {
            if (MATCH(name, "strength")) {
                cfg->lumaDenoise.strength = f;

                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //ADAPTATION:
                //Derive LUT and BITPOS based on strength
                lumaGenLut(cfg->lumaDenoise.strength, 
                    cfg->lumaDenoise.lut, 
                    (int*)&cfg->lumaDenoise.bitpos);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            } else {
                error = 1;
            }
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "Luma Denoise Ref")) {
            if (MATCH(name, "angle_of_view")) {
                cfg->lumaDenoiseRef.angle_of_view = f;
                luma_denoise_ref_gen_lut_dist(cfg, f/*angle*/, iCfg->iSize.w, iCfg->iSize.h);
            } else if (MATCH(name, "gamma")) {
                cfg->lumaDenoiseRef.gamma = f;
                luma_denoise_ref_gen_lut_gamma(cfg, f/*gamma*/);
            } else {
                error = 1;
            }


            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "Sharpen")) {
            if (MATCH(name, "strength_darken")) {
                //cfg->sharpen.strength_darken = f;
                cfg->sharpen.strength_darken = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "strength_lighten")) {
                //cfg->sharpen.strength_lighten = f;
                cfg->sharpen.strength_lighten = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "sigma")) {
                cfg->sharpen.sigma = f;

                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //ADAPTATION:
                //Derive sharpen coefs based on sigma
                sharpenSigmaToCoefficients(cfg->sharpen.sigma, 
                    cfg->sharpen.sharpen_coeffs);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            } else if (MATCH(name, "alpha")) {
                //cfg->sharpen.alpha = f;
                cfg->sharpen.alpha = f32_to_f16(f); //ADAPTATION
            }else if (MATCH(name, "overshoot")) {
                //cfg->sharpen.overshoot = f;
                cfg->sharpen.overshoot = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "undershoot")) {
                //cfg->sharpen.undershoot = f;
                cfg->sharpen.undershoot = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "range_stop_0")) {
                //cfg->sharpen.rangeStop0 = f;
                cfg->sharpen.rangeStop0 = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "range_stop_1")) {
                //cfg->sharpen.rangeStop1 = f;
                cfg->sharpen.rangeStop1 = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "range_stop_2")) {
                //cfg->sharpen.rangeStop2 = f;
                cfg->sharpen.rangeStop2 = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "range_stop_3")) {
                //cfg->sharpen.rangeStop3 = f;
                cfg->sharpen.rangeStop3 = f32_to_f16(f); //ADAPTATION
            } else if (MATCH(name, "min_thr")) {
                //cfg->sharpen.minThr = f;
                cfg->sharpen.minThr = f32_to_f16(f); //ADAPTATION
            } else {
                error = 1;
            }

            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "Random Noise")) {
            if (MATCH(name, "strength")) {
                cfg->randomNoise.strength = f;
            } else {
                error = 1;
            }

            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "Median Mix")) {
            if (MATCH(name, "slope")) {
                cfg->medianMix.slope = f;
            } else if (MATCH(name, "offset")) {
                cfg->medianMix.offset = f;
            } else {
                error = 1;
            }

            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "LowPass")) {
            if (MATCH(name, "convolution")) {
                if (size != 2) {
                    fprintf(stderr, "Convolution should be 2 values (separable 3x3 mat)\n");
                    error = 1;
                } else {
                    for (i = 0; i < 2; i++) {
                        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                        //ADAPTATION:
                        cfg->lowpass.coefs[i] = (((float *)val)[i]);
                        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                    }
                }
            } else {
                error = 1;
            }

            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else if (MATCH(section, "Color Combine")) {
            if (MATCH(name, "ccm")) {
                if (size != 9) {
                    fprintf(stderr, "CCM should be 3x3 (9-value array)\n");
                    error = 1;
                } else {
                    for (i = 0; i < 9; i++) {
                        cfg->colorCombine.ccm[i] = ((float *)val)[i];
                    }
                }
            } else if (MATCH(name, "desat_t1")) {
                cfg->colorCombine.desat_t1 = f;
            } else if (MATCH(name, "desat_mul")) {
                cfg->colorCombine.desat_mul = f;
            } else {
                error = 1;
            }
        } 

        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        else if (MATCH(section, "Color Convert")) {
            if (MATCH(name, "mat")) {
                if (size != 9) {
                    fprintf(stderr, "CCM should be 3x3 (9-value array)\n");
                    error = 1;
                } else {
                    for (i = 0; i < 9; i++) {
                        cfg->colorConvert.mat[i] = ((float *)val)[i];
                    }
                }
            } else if (MATCH(name, "offset")) {
                cfg->colorConvert.offset[0] = ((float *)val)[0];
                cfg->colorConvert.offset[1] = ((float *)val)[1];
                cfg->colorConvert.offset[2] = ((float *)val)[2];
            } else {
                error = 1;
            }
        } 

        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        else if (MATCH(section, "Gamma")) {

            if (MATCH(name, "gamma_table")) {
                if (size > (int)(sizeof(HrLut) / sizeof(HrLut[0]))) {
                    fprintf(stderr, "Gamma Table in config file is too big\n");
                    error = 1;
                } else {
                    HrLutSize = size;
                    for(i = 0; i < size; i++) {
                        HrLut[i] = ((float *)val)[i];
                    }  
                }

                //TBD: do all alocs in a separate func (that includes checks)
                if ((cfg->gamma.table = (uint16_t *)malloc(HrLutTargetSize *
                    sizeof (uint16_t)*4)) == NULL) {
                        perror("malloc");
                        abort();
                }

                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //ADAPTATION
                //Convert from human readable to SIPP format
                sippUtilComputeFp16LutChannelMode(lutFormula, 
                    (half*)cfg->gamma.table,
                    cfg->gamma.size);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

            } else {
                error = 1;
            }
        } 

        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        else if (MATCH(section, "Local Tone Mapping")) 
        {
            if (MATCH(name, "curves")) 
            {
                //if (size != (int)(sizeof(cfg->ltm.ltmCurves) / sizeof(cfg->ltm.ltmCurves[0]))) 
                //local hack: I know .text file only has 15x9 entries, but for ipipe purposes it's defined as 16x10
                if (size != 15 * 9)  
                {
                    fprintf(stderr, "Local Tone Mapping curves not of expected size\n");
                    error = 1;
                } 
                else {
                    for (i = 0; i < size; i++) {
                        HrLtmCurves[i] = ((float *)val)[i];
                    }

                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    //ADAPTATION:
                    convertLtmCurves(HrLtmCurves, cfg->ltm.curves);
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~

                }
            } else {
                error = 1;
            }

            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        } else {
            error = 1;
        }
        break;
                         }


    case INI_TYPE_STRING:
        if (MATCH(section, "General")) {
            if (MATCH(name, "filename")) {
                printf("Input filename: %s\n", (char *)val);
                iCfg->inputFilename = STRDUP((char *)val);
            } else if (MATCH(name, "bayer_order")) {
                if (MATCH((char *)val, "GR")) {
                    cfg->bayerOrder = IC_BAYER_FORMAT_GRBG;
                } else if (MATCH((char *)val, "GB")) {
                    cfg->bayerOrder = IC_BAYER_FORMAT_GBRG;
                } else if (MATCH((char *)val, "RG")) {
                    cfg->bayerOrder = IC_BAYER_FORMAT_RGGB;
                } else if (MATCH((char *)val, "BG")) {
                    cfg->bayerOrder = IC_BAYER_FORMAT_BGGR;
                } else {
                    error = 1;
                }
            }
        } else {
            error = 1;
        }
        break;


    default:
        fprintf(stderr, "Unknown data type %d\n", type);
        return -1;
    }

    if (error) {
        fprintf(stderr, "Unrecognized or bad parameter %s in section %s\n",
            name, section);
    }

    return 0;
}

//##########################################################################
int parseIniFile(const char *fname, ispCfg *argCfg)
{
    FILE	*f = fopen(fname, "rb");
    char	buf[1024];
    int	rc = 0;

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    iCfg           = argCfg; //keep ref
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //some defaults
    iCfg->svuFirst =      0;
    iCfg->svuLast  =     11;
    iCfg->svuElf   =   NULL;
    //If not present in config file, assume 1
    iCfg->ispParams->chromaGen.scaleR = 1.0f;
    iCfg->ispParams->chromaGen.scaleG = 1.0f;
    iCfg->ispParams->chromaGen.scaleB = 1.0f;
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    section_name = NULL; //init

    if (f == NULL) {
        perror(fname);
        return -1;
    }

    lno = 0;
    while (fgets(buf, sizeof (buf), f) != NULL) {
        lno++;
        buf[sizeof (buf) - 1] = '\0';
        if (parse_line(f, buf, callback) == -1) {
            fprintf(stderr, "Parse error on line %d\n", lno);
            rc = -1;
            break;
        }
    }

    if (section_name != NULL) {
        free(section_name);
    }
    fclose(f);
    return rc;
}

//######################################################################
void parseAppArgs(int argc, char **argv, int *dbgLevel, char **cfgFileName)
{
#ifdef SIPP_PC
    int  usage = 0;

    switch (argc) 
    {
    case 1:
        break;
    case 2:
        if (argv[1][0] == '-') 
        {
            if (argv[1][1] == 'd') 
            {
                *dbgLevel = strtol(&argv[1][2], NULL, 0);
            } 
            else 
            {
                usage = 1;
            }
        }
        else
            *cfgFileName = argv[1];
        break;
    case 3:
        if (argv[1][0] == '-') 
        {
            if (argv[1][1] == 'd') 
            {
                *dbgLevel = strtol(&argv[1][2], NULL, 0);
            } 
            else 
            {
                usage = 1;
            }
        }
        else
            usage = 1;

        *cfgFileName = argv[2];

        break;
    default:
        usage = 1;
        break;
    }

    if (usage) {
        fprintf(stderr, "Usage: %s [-dN] [config_file]\n", argv[0]);
        fprintf(stderr, "'N' specifies the debug level\n");
        exit(1);
    }
#endif
}

//################################################################################################################
//Still pipeline Read + Checks wrapper
void stillGetConfig(const char *cfgFile, ispCfg *iCfg)
{
    icIspConfig *cfg = iCfg->ispParams;

    // Settings come from config file
    if (parseIniFile(cfgFile, iCfg)  ==   -1) {  printf("Error parsing config file\n");                   abort(); }
    if (cfg->ltm.curves              == NULL) {  printf("No LTM curves in config file\n");                abort(); }
    if (cfg->gamma.table             == NULL) {  printf("No Gamma table in config file\n");               abort(); }
    if (cfg->colorCombine.ccm        == NULL) {  printf("No Color Matrix in config file\n");              abort(); }
    if (cfg->lowpass.coefs           == NULL) {  printf("No low-pass kernel in config file\n");           abort(); }
    if (cfg->lumaDenoiseRef.lutDist  == NULL) {  printf("No Luma Denoise Distance LUT in config file\n"); abort(); }
    if (cfg->lumaDenoiseRef.lutGamma == NULL) {  printf("No Luma Denoise Gamma LUT in config file\n");    abort(); }
}





//################################################################################################################
static void dumpGammaTable(ispCfg *iCfg, FILE *f)
{
    uint32_t t;
    icIspConfig *iParams = iCfg->ispParams;

    fprintf(f, "static uint16_t lutTable[] ALIGNED(8) = { //fp16  CHANNEL-MODE \n");
    for(t=0; t<iCfg->ispParams->gamma.size/4; t++)
    {
        fprintf(f, "     0x%04x,0x%04x,0x%04x,0x%04x,  0x%04x,0x%04x,0x%04x,0x%04x,  0x%04x,0x%04x,0x%04x,0x%04x,  0x%04x,0x%04x,0x%04x,0x%04x, \n", 
            iParams->gamma.table[t*4*4+ 0],
            iParams->gamma.table[t*4*4+ 1],
            iParams->gamma.table[t*4*4+ 2],
            iParams->gamma.table[t*4*4+ 3],
            iParams->gamma.table[t*4*4+ 4],
            iParams->gamma.table[t*4*4+ 5],
            iParams->gamma.table[t*4*4+ 6],
            iParams->gamma.table[t*4*4+ 7],
            iParams->gamma.table[t*4*4+ 8],
            iParams->gamma.table[t*4*4+ 9],
            iParams->gamma.table[t*4*4+10],
            iParams->gamma.table[t*4*4+11],
            iParams->gamma.table[t*4*4+12],
            iParams->gamma.table[t*4*4+13],
            iParams->gamma.table[t*4*4+14],
            iParams->gamma.table[t*4*4+15]);
    }
    fprintf(f, "}; \n\n\n");
}
//################################################################################################################
static void dumpIcIspConfig(ispCfg *iCfg, FILE *f, char *cfgName)
{
    int t;
    icIspConfig *iParams = iCfg->ispParams;

    fprintf(f, "icIspConfig %s ALIGNED(8) = \n", cfgName);
    fprintf(f, "{\n");

    fprintf(f, " 0,             //.frameCount;          \n");
    fprintf(f, " 0,             //.frameId;             \n");
    fprintf(f, " 0/*null*/,     //.*userData;           \n");
    fprintf(f, " 0,             //.dirtyFlags;          \n");
    fprintf(f, " 0,             //.enableFlags          \n");
    fprintf(f, " 0,             //.pipeControl          \n");
    fprintf(f, " %d,            //.pipelineBits         \n\n", iParams->pipelineBits);
    fprintf(f, " %d,            //.bayerOrder           \n\n", iParams->bayerOrder);


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0, 0, 0, 0},     //blc                \n\n"); //.blc

    //.raw UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0, 0, 0, 0,      //raw.gainXX         \n");
    fprintf(f, "  0, 0, 0, 0,      //raw.clampXX        \n");
    fprintf(f, "  0, 0, 0, 0, 0,   //raw.grgbImbalXX    \n");
    fprintf(f, "  0, 0, 0, 0, 0,   //raw.defectPix      \n");
    fprintf(f, "  0  },            //raw.outputBits     \n\n");

    //.lsc UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0, 0, 0, 0/*null*/},  //lsc \n\n");

    //.wbGains
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0x%04x, 0x%04x, 0x%04x },  //wbGains.gainsXX  \n\n",
        iParams->wbGains.gainR, 
        iParams->wbGains.gainG,
        iParams->wbGains.gainB);

    //.demosaic: just in STILL pipe
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0x%x, 0x%x, 0x%x },  //demosaic.   \n\n",
        iParams->demosaic.dewormGradientMul,
        iParams->demosaic.dewormSlope,		
        iParams->demosaic.dewormOffset);

    //.aeAwbConfig : UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{0,0,0,0, 0,0,0,0, 0},   //aeAwbConfig \n\n");

    //.afConfig : UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0,0,0,0, 0,0,0,0, 0,    //afConfig   \n");
    fprintf(f, "  {0,0,0,0,0,0,0,0,0,0,0},             \n");
    fprintf(f, "  0,                                   \n");
    fprintf(f, "  {0,0,0,0,0,0,0,0,0,0,0} },           \n\n");

    //.aeAwbStats/afStats : UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "0/*null*/,                //aeAwbStats \n\n");
    fprintf(f, "0/*null*/,                //afStats    \n\n");

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %f, %f, %f, %f },  //chromaGen  \n\n",
        iParams->chromaGen.epsilon,
        iParams->chromaGen.scaleR,
        iParams->chromaGen.scaleG, 
        iParams->chromaGen.scaleB); 

    //.purpleFlare UNUSED
    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ 0 },  // purpleFlare.strength \n\n");


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d,                       //lumaDenoise.alpha    \n", iParams->lumaDenoise.alpha);
    fprintf(f, "  %f,                       //lumaDenoise.strength \n", iParams->lumaDenoise.strength);
    fprintf(f, "  %d,                       //lumaDenoise.bitpos   \n", iParams->lumaDenoise.bitpos);
    fprintf(f, " {//lumaDenoise.lut                                \n");
    for(t=0; t<4; t++)
    {
        fprintf(f, " %3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d, \n", 
            iParams->lumaDenoise.lut[t*8+0],
            iParams->lumaDenoise.lut[t*8+1],
            iParams->lumaDenoise.lut[t*8+2],
            iParams->lumaDenoise.lut[t*8+3],
            iParams->lumaDenoise.lut[t*8+4],
            iParams->lumaDenoise.lut[t*8+5],
            iParams->lumaDenoise.lut[t*8+6],
            iParams->lumaDenoise.lut[t*8+7]);
    }
    fprintf(f, " },   \n");
    fprintf(f, "0x%x },  //lumaDenoise.f2 \n", iParams->lumaDenoise.f2);


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{                           //lumaDenoiseRef           \n");
    //-----------------------------------------------------------------------------------
    fprintf(f, "  %f, %f, //angle-of-view, gamma \n", 
        iParams->lumaDenoiseRef.angle_of_view, 
        iParams->lumaDenoiseRef.gamma);
    //-----------------------------------------------------------------------------------
    fprintf(f, "  {//lumaDenoiseRef.lutDist \n");
    for(t=0; t<256/8; t++){
        fprintf(f, "     0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x, \n", 
            iParams->lumaDenoiseRef.lutDist[t*8+0],
            iParams->lumaDenoiseRef.lutDist[t*8+1],
            iParams->lumaDenoiseRef.lutDist[t*8+2],
            iParams->lumaDenoiseRef.lutDist[t*8+3],
            iParams->lumaDenoiseRef.lutDist[t*8+4],
            iParams->lumaDenoiseRef.lutDist[t*8+5],
            iParams->lumaDenoiseRef.lutDist[t*8+6],
            iParams->lumaDenoiseRef.lutDist[t*8+7]);
    }
    fprintf(f, "  },\n\n");

    //-----------------------------------------------------------------------------------
    fprintf(f, "  {//lumaDenoiseRef.lutGamma START  \n");
    for(t=0; t<256/8; t++){
        fprintf(f, "     0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x, \n", 
            iParams->lumaDenoiseRef.lutGamma[t*8+0],
            iParams->lumaDenoiseRef.lutGamma[t*8+1],
            iParams->lumaDenoiseRef.lutGamma[t*8+2],
            iParams->lumaDenoiseRef.lutGamma[t*8+3],
            iParams->lumaDenoiseRef.lutGamma[t*8+4],
            iParams->lumaDenoiseRef.lutGamma[t*8+5],
            iParams->lumaDenoiseRef.lutGamma[t*8+6],
            iParams->lumaDenoiseRef.lutGamma[t*8+7]);
    }
    fprintf(f, "  },\n\n");
    //-----------------------------------------------------------------------------------
    fprintf(f, " %d }, //shift    \n\n", iParams->lumaDenoiseRef.shift);


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{  %f },                         //randomNoise.strength    \n\n", iParams->randomNoise.strength);



    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ {//ltm.ltmCurves[16*10]  \n");
    for(t=0; t<10; t++)
    {
        fprintf(f, "     0x%04x,0x%04x,0x%04x,0x%04x, 0x%04x,0x%04x,0x%04x,0x%04x, 0x%04x,0x%04x,0x%04x,0x%04x, 0x%04x,0x%04x,0x%04x,0x%04x, \n", 
            iParams->ltm.curves[t*16+ 0],
            iParams->ltm.curves[t*16+ 1],
            iParams->ltm.curves[t*16+ 2],
            iParams->ltm.curves[t*16+ 3],
            iParams->ltm.curves[t*16+ 4],
            iParams->ltm.curves[t*16+ 5],
            iParams->ltm.curves[t*16+ 6],
            iParams->ltm.curves[t*16+ 7],
            iParams->ltm.curves[t*16+ 8],
            iParams->ltm.curves[t*16+ 9],
            iParams->ltm.curves[t*16+10],
            iParams->ltm.curves[t*16+11],
            iParams->ltm.curves[t*16+12],
            iParams->ltm.curves[t*16+13],
            iParams->ltm.curves[t*16+14],
            iParams->ltm.curves[t*16+15]);
    }
    fprintf(f, "} },  \n\n");

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d, %d, %d },  //chromaDenoiseLtm  \n\n",
        iParams->ltmLBFilter.th1,
        iParams->ltmLBFilter.th2,
        iParams->ltmLBFilter.limit);

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d, %d, %d, %d, %d },  //chromaDenoise  \n\n",
        iParams->chromaDenoise.th_r,
        iParams->chromaDenoise.th_g,
        iParams->chromaDenoise.th_b,
        iParams->chromaDenoise.limit,
        iParams->chromaDenoise.hEnab);

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d },                 //chromaMedian.kernelSize  \n\n",
        iParams->median.kernelSize);

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %f, %f },         //medianMix  \n\n",
        iParams->medianMix.slope,
        iParams->medianMix.offset);

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ { %f, %f} }, //lowpass  \n\n",
        iParams->lowpass.coefs[0],
        iParams->lowpass.coefs[1] );


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d, %d },         //greyDesat  \n\n",
        iParams->greyDesat.slope,
        iParams->greyDesat.offset);

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %f,                   //sharpen.sigma            \n", iParams->sharpen.sigma);
    fprintf(f, "  {0x%04x,0x%04x,0x%04x,0x%04x},  //sharpen.sharpen_coef[4]  \n", 
        iParams->sharpen.sharpen_coeffs[0],
        iParams->sharpen.sharpen_coeffs[1],
        iParams->sharpen.sharpen_coeffs[2],
        iParams->sharpen.sharpen_coeffs[3]);
    fprintf(f, "   0x%04x,                     //sharpen.strength_darken;  \n", iParams->sharpen.strength_darken);
    fprintf(f, "   0x%04x,                     //sharpen.strength_lighten; \n", iParams->sharpen.strength_lighten);
    fprintf(f, "   0x%04x,                     //sharpen.alpha;            \n", iParams->sharpen.alpha);
    fprintf(f, "   0x%04x,                     //sharpen.overshoot;        \n", iParams->sharpen.overshoot);
    fprintf(f, "   0x%04x,                     //sharpen.undershoot;       \n", iParams->sharpen.undershoot);
    fprintf(f, "   0x%04x,                     //sharpen.rangeStop0;       \n", iParams->sharpen.rangeStop0);
    fprintf(f, "   0x%04x,                     //sharpen.rangeStop1;       \n", iParams->sharpen.rangeStop1);
    fprintf(f, "   0x%04x,                     //sharpen.rangeStop2;       \n", iParams->sharpen.rangeStop2);
    fprintf(f, "   0x%04x,                     //sharpen.rangeStop3;       \n", iParams->sharpen.rangeStop3);
    fprintf(f, "   0x%04x,                     //sharpen.minThr;           \n", iParams->sharpen.minThr);
    fprintf(f, " },\n\n");

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %f, %f,         //colComb  \n",
        iParams->colorCombine.desat_mul,
        iParams->colorCombine.desat_t1);
    fprintf(f, " { //ccm[3x3]  \n");
    for(t=0; t<3; t++)
    {
        fprintf(f, "     %+f, %+f, %+f, \n", 
            iParams->colorCombine.ccm[t*3+ 0],
            iParams->colorCombine.ccm[t*3+ 1],
            iParams->colorCombine.ccm[t*3+ 2]);                
    }
    fprintf(f, "  }, \n"); //close ccm
    fprintf(f, "}, \n\n"); //close colComb

    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{ %d, %d, &lutTable[0]},       //gamma  \n\n", iParams->gamma.rangetop,
        iParams->gamma.size);


    fprintf(f, "//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(f, "{//colorConvert \n");
    fprintf(f, " { //mat[3x3]  \n");
    for(t=0; t<3; t++)
    {
        fprintf(f, "     %+f, %+f, %+f, \n", 
            iParams->colorConvert.mat[t*3+ 0],
            iParams->colorConvert.mat[t*3+ 1],
            iParams->colorConvert.mat[t*3+ 2]);                
    }
    fprintf(f, "  }, \n"); //close ccm

    fprintf(f, " { %f, %f, %f} //offset[3]  \n",
        iParams->colorConvert.offset[0],
        iParams->colorConvert.offset[1],
        iParams->colorConvert.offset[2]);
    fprintf(f, "}, \n\n"); //close colConvert


    fprintf(f, "};");

}
//################################################################################################################
// IPIPE SIPP pipelines don't use RAW and LSC blocks !
void dumpIspConfig(ispCfg *iCfg, char *cfgName)
{
    FILE *f = fopen("ipipeTestParams.c", "w");

    fprintf(f, "//####################################################################\n");
    fprintf(f, "//AUTOMATICALLY DUMPED FROM PC APP !\n");
    fprintf(f, "// From file : %s \n", __FILE__);
    fprintf(f, "// Function  : %s \n", __FUNCTION__);
    fprintf(f, "//####################################################################\n\n\n");

    fprintf(f, "#include <common/leon/ipipe.h>     \n\n");

    fprintf(f, "#if defined(__sparc)                             \n");
    fprintf(f, "  #define ALIGNED(x) __attribute__((aligned(x))) \n");
    fprintf(f, "#else // PC world                                \n");
    fprintf(f, "  #define ALIGNED(x) //nothing                   \n");
    fprintf(f, "#endif                                           \n\n");

    dumpGammaTable (iCfg, f);
    dumpIcIspConfig(iCfg, f, cfgName);

    fclose(f);
}

#endif
