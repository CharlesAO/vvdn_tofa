#include "opticalFlowPyrLK.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#include <mvcv.h>
#include <mvcv_types.h>
#include <mat.h>
#include "kernels.h"
#include "samplers.h"

#define PYR_LK_PRINTF(...) //printf(__VA_ARGS__)

static inline void intersect_tuned(mvCvPoint2D32fW pt, ClSizeW win_size,
                                   ClSizeW imgSize,
                                   mvCvPointW* min_pt,
                                   mvCvPointW* max_pt)
{
    mvCvPointW ipt;
    ipt.x = mvcvFloor(pt.x);
    ipt.y = mvcvFloor(pt.y);

    ipt.x = ipt.x - win_size.x;
    ipt.y = ipt.y - win_size.y;

    win_size.x = win_size.x * 2 + 1;
    win_size.y = win_size.y * 2 + 1;

#ifdef __PC__
    min_pt->x = MAX( 0, -ipt.x );
    min_pt->y = MAX( 0, -ipt.y );
    max_pt->x = MIN( win_size.x, imgSize.x - ipt.x );
    max_pt->y = MIN( win_size.y, imgSize.y - ipt.y );
#else
    *min_pt = __builtin_shave_cmu_max_i32_rr_int4((int4) 0, -ipt);
    *max_pt = __builtin_shave_cmu_min_i32_rr_int4(win_size, imgSize - ipt);
#endif
}

s32 mvcvCalcOpticalFlowPyrLK_tuned(Mat pyrA[], Mat pyrB[],
                                   mvCvPoint2D32f* featuresA,
                                   mvCvPoint2D32f* featuresB,
                                   u8 *status,
                                   fp32 *error, mvCvSize _winSize, u32 levels,
                                   mvCvTermCriteria criteria, u32 flags,
                                   u32 count)
{
    // Normalized Scharr kernel
    static float smoothKernel[] =
    {
        0.09375,
        0.3125,
        0.09375
    };  // 3/32, 10/32, 3/32
    static const u32 xx = 0;
    static const u32 yy = 1;
    static const u32 xy = 2;
    static const u32 D = 3;

    ClSizeW winSize = clSizeW(_winSize.width, _winSize.height);

    s32 l;
    u32 i;
    u32 level = levels;

    // Check input arguments
    if (!featuresA || !featuresB)
        return MVCV_NULLPTR_ERR;

    if (winSize.x > 24 || winSize.y > 24)
        return MVCV_BADSIZE_ERR;

    if ((flags & ~0x1F) != 0)
        return MVCV_BADFLAG_ERR;

    if (count == 0)
        return MVCV_BADRANGE_ERR;
    // Initialize status
    for (i = 0; i < count; i++)
    {
        status[i] = 1;
        error[i] = 0;
    }

    // Initialize guesses
    if (!(flags & MVCV_LKFLOW_INITIAL_GUESSES))
        for (i = 0; i < count; i++)
        {
            featuresB[i].x = featuresA[i].x;
            featuresB[i].y = featuresA[i].y;
        }

    // Scale coordinates to the highest pyramid level
    for (i = 0; i < count; i++)
    {
        const float scale = 1.f / (1 << level) * 0.5f;
        featuresB[i].x = featuresB[i].x * scale;
        featuresB[i].y = featuresB[i].y * scale;
    }

    mvCvSize patchSize = mvcvSize(winSize.x * 2 + 1, winSize.y * 2 + 1);
    int patchLen = patchSize.width * patchSize.height;
    int srcPatchLen = (patchSize.width + 2) * (patchSize.height + 2);

#ifdef __PC__
    u8* patchI = (u8*)malloc(srcPatchLen * sizeof(patchI[0]));
    u8* patchJ = (u8*)malloc(patchLen * sizeof(patchJ[0]));
    float* Ix = (float*)malloc(patchLen * sizeof(Ix[0]));
    float* Iy = (float*)malloc(patchLen * sizeof(Iy[0]));
#else
    u8 patchI[srcPatchLen * sizeof(u8)];
    u8 patchJ[patchLen * sizeof(u8)];
    float Ix[patchLen];
    float Iy[patchLen];
#endif


    // Do processing from top pyramid level (smallest image)
    // to the bottom (original image)
    for (l = levels; l >= 0; l--)
    {
        float scaleL = 1.f / (1 << l);
        ClSizeW levelSize = clSizeW(pyrA[l].cols, pyrA[l].rows);

        // Find flow for each given point
        for (i = 0; i < count; i++)
        {
            if (status[i] == 0)
                continue;
            PYR_LK_PRINTF("	%d:%d\n", l, i);

            mvCvPoint2D32fW v;
            mvCvPointW minI, maxI, minJ, maxJ;
            ClSizeW isz, jsz;
            int pt_status;
            mvCvPoint2D32fW u;
            mvCvPointW prev_minJ = mvcvPointW(-1, -1);
            mvCvPointW prev_maxJ = mvcvPointW(-1, -1);
            float prev_mx = 0;
            float prev_my = 0;
            u32 j;
            v.x = featuresB[i].x * 2;
            v.y = featuresB[i].y * 2;
            PYR_LK_PRINTF("		%d\n", i);
            PYR_LK_PRINTF("		v(%f %f) f(%f %f)\n", v.x, v.y, featuresB[i].x, featuresB[i].y);
            pt_status = status[i];
            minI = maxI = minJ = maxJ = mvcvPointW(0, 0);
            u.x = featuresA[i].x * scaleL;
            u.y = featuresA[i].y * scaleL;
            PYR_LK_PRINTF("		u: %f %f\n", u.x, u.y);
            PYR_LK_PRINTF("%d %d %d %d\n", maxI.x, maxI.y, minI.x, minI.y);
            intersect_tuned(u, winSize, levelSize, &minI, &maxI);
            PYR_LK_PRINTF("%d %d %d %d\n", maxI.x, maxI.y, minI.x, minI.y);
            isz.x = maxI.x - minI.x + 2;
            isz.y = maxI.y - minI.y + 2;
            u.x += (minI.x - (patchSize.width - maxI.x + 1)) * 0.5f;
            u.y += (minI.y - (patchSize.height - maxI.y + 1)) * 0.5f;
            PYR_LK_PRINTF("		u: %f %f\n", u.x, u.y);
            PYR_LK_PRINTF("     isz: %d %d\n", isz.x, isz.y);
            if (isz.x < 3 || isz.y < 3)
            {
                // Point is outside the first image. take the next
                PYR_LK_PRINTF("		off1\n");
                status[i] = 0;
                //error[i] = 0;
                error[i] = 9999999.9f;
                continue;
            }
            icvGetRectSubPix_8u32f_C1R_tuned(
                            pyrA[l].data, pyrA[l].step, levelSize,
                            patchI,
                            isz.x * sizeof(patchI[0]), isz, u,
                            flags & MVCV_LKFLOW_PADDED_PYRAMIDS);
            PYR_LK_PRINTF("		u: %f %f\n", u.x, u.y);

            // Vectorize
            mvcvConv3x3fp32Scharr_asm(patchI, Ix, Iy, smoothKernel,
                                      isz.y , isz.x);
            mvScalar G = mvcvScalar(0.0, 0.0, 0.0, 0.0);
            for (j = 0; j < criteria.max_iter; j++)
            {
                float mx, my;
                mvCvPoint2D32fW _v;
                intersect_tuned(v, winSize, levelSize, &minJ, &maxJ);
#ifdef __PC__
                minJ.x = MAX( minJ.x, minI.x );
                minJ.y = MAX( minJ.y, minI.y );

                maxJ.x = MIN( maxJ.x, maxI.x );
                maxJ.y = MIN( maxJ.y, maxI.y );
#else
                minJ = __builtin_shave_cmu_max_i32_rr_int4(minJ, minI);
                maxJ = __builtin_shave_cmu_min_i32_rr_int4(maxJ, maxI);
#endif
                jsz.x = maxJ.x - minJ.x;
                jsz.y = maxJ.y - minJ.y;
                PYR_LK_PRINTF("		v:(%f %f) minJ(%d %d) maxJ(%d %d)\n",
                                v.x, v.y, minJ.x, minJ.y, maxJ.x, maxJ.y);
                _v.x = v.x + (minJ.x - (patchSize.width - maxJ.x + 1)) * 0.5f;
                _v.y = v.y + (minJ.y - (patchSize.height - maxJ.y + 1)) * 0.5f;
                PYR_LK_PRINTF("		_v: %f %f\n", _v.x, _v.y);
                // Vectorize
                if (jsz.x < 1 || jsz.y < 1)
                {
                    // point is outside of the second image. take the next
                    PYR_LK_PRINTF("		off2\n");
                    pt_status = 0;
                    //error[i] = 0
                    error[i] = 9999999.9f;
                    break;
                }
                icvGetRectSubPix_8u32f_C1R_tuned(
                                pyrB[l].data, pyrB[l].step, levelSize,
                                patchJ,
                                jsz.x * sizeof(patchJ[0]), jsz, _v,
                                flags & MVCV_LKFLOW_PADDED_PYRAMIDS);
                // Compute mismatch vector
                mvCvPoint2D32f b;
                if (jsz.x < 4)
                    b = mvcvCalcBxBy(patchI, isz, Ix, Iy, minI,
                                     patchJ, jsz, minJ,
                                     &error[i]);
                else
                    b = mvcvCalcBxBy_asm(patchI, isz, Ix, Iy, minI,
                                         patchJ, jsz, minJ,
                                         &error[i]);
                if (l == 0)
                    error[i] = fastSqrt(error[i]);
                // Check if the candidate point has moved more than 1 pixel
                // after last iteration's guess. If it did, we need to recompute
                // the gradients matrix.
                if (maxJ.x != prev_maxJ.x || maxJ.y != prev_maxJ.y ||
                    minJ.x != prev_minJ.x || minJ.y != prev_minJ.y)
                {
                    // Compute gradients matrix and its determinant (eq. 24)
                    if (jsz.x >= 4 && jsz.y >= 4)
                        mvcvCalcG_asm(Ix, isz, minI, Iy, jsz,
                                      minJ, G);
                    else
                        mvcvCalcG(Ix, isz, minI, Iy, jsz,
                                  minJ, G);
                    // If determinant is too close to 0, the matrix is not
                    // invertible, therefore the motion vector can't be calculated
                    if (G[D] < DBL_EPSILON)
                    {
                        PYR_LK_PRINTF("		G\n");
                        pt_status = 0;
                        error[i] = 9999999.9f;
                        break;
                    }
                    G[D] = 1.f / G[D];
                    prev_minJ = minJ;
                    prev_maxJ = maxJ;
                }
                // Compute the optical flow vector by solving (eq. 25)
                mx = (float) ((G[yy] * b.x - G[xy] * b.y) * G[D]);
                my = (float) ((G[xx] * b.y - G[xy] * b.x) * G[D]);
                // Make the guess for the next iteration
                v.x += mx;
                v.y += my;
                float m = mx * mx + my * my;
                PYR_LK_PRINTF("		eps: %f < %f\n", m, criteria.epsilon);
                if (m < criteria.epsilon)
                {
                    PYR_LK_PRINTF("		eps\n");
                    break;
                }

                // If the point moved too little, we found it
#ifdef __PC__
                if( j > 0 && fabs(mx + prev_mx) < 0.01 && fabs(my + prev_my) < 0.01 )
#else
                if (j > 0 && __builtin_shave_sau_abs_f32_r(mx + prev_mx) < 0.01
                    && __builtin_shave_sau_abs_f32_r(my + prev_my) < 0.01)
                #endif
                {
                    PYR_LK_PRINTF("		mv\n");
                    v.x -= mx * 0.5f;
                    v.y -= my * 0.5f;
                    break;
                }
                // Hold the last motion vector
                prev_mx = mx;
                prev_my = my;
            }
            PYR_LK_PRINTF("	end pt %d\n", i);
            // If the point slips off the image, its lost
            if (v.x < 0 || v.y < 0 || v.x >= levelSize.x || v.y >= levelSize.y)
            {
                PYR_LK_PRINTF("	off\n");
                pt_status = 0;
                error[i] = 9999999.9f;
            }
            featuresB[i].x = v.x;
            featuresB[i].y = v.y;
            PYR_LK_PRINTF("f(%f %f)\n", featuresB[i].x, featuresB[i].y);
            status[i] = (char) pt_status;
        } // end of point processing loop (i)
        PYR_LK_PRINTF("end %d:%d\n", l, i);
    }
    PYR_LK_PRINTF("done\n");

#ifdef __PC__
    free(patchI);
    free(patchJ);
    free(Ix);
    free(Iy);
#endif
    return MVCV_OK;
}
