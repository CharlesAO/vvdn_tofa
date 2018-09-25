#include "samplers.h"

#include <string.h>

#include <mvcv.h>
#include <mvcv_types.h>
#include "kernels.h"

// PC Version
#if defined(__PC__)
static inline void
smartCopy(u8* dst, u8* src, u32 numBytes, u32 width = 0, s32 stride = 0)
{
    u32 height = (width == 0) ? 1 : (numBytes / width);
    u32 len = (width == 0) ? numBytes : width;
    for (u32 i = 0; i < height; i++, src += (len + stride), dst += len)
    {
        for (u32 j = 0; j < len; j++)
        {
            dst[j] = src[j];
    }
    }
}
#else // Myriad2 version
static inline void
smartCopy(u8* dst, u8* src, u32 numBytes, u32 width = 0, s32 stride = 0)
{
    u64 localData[4];
    {
        u32 height = (width == 0) ? 1 : (numBytes / width);
        u32 len = (width == 0) ? numBytes : width;
        for (u32 i = 0; i < height;
                        i++, src += stride, dst += len)
        {
            u64* alignedSrc = (u64*)((u32)src & 0xFFFFFFF8);
            u32 alignOffset = (u32)src & 0x7;

            localData[0] = alignedSrc[0];
            localData[1] = alignedSrc[1];
            localData[2] = alignedSrc[2];
            localData[3] = alignedSrc[3];
            for (u32 j = 0; j < len; j++)
            {
                dst[j] = ((u8*)localData)[j + alignOffset];
            }
        }
    }
}
#endif

static inline const void*
icvAdjustRect_tuned(const void* srcptr, int src_step, int pix_size,
                    ClSizeW src_size,
                    ClSizeW win_size,
                    mvCvPointW ip,
                    mvCvRect* pRect)
{
    mvCvRect rect;
    const char* src = (const char*) srcptr;

    if (ip.x >= 0)
    {
        src += ip.x * pix_size;
        rect.x = 0;
    }
    else
    {
        rect.x = -ip.x;
        if (rect.x > win_size.x)
            rect.x = win_size.x;
    }

    if (ip.x + win_size.x < src_size.x)
        rect.width = win_size.x;
    else
    {
        rect.width = src_size.x - ip.x - 1;
        if (rect.width < 0)
        {
            src += rect.width * pix_size;
            rect.width = 0;
        }
    }

    if (ip.y >= 0)
    {
        src += ip.y * src_step;
        rect.y = 0;
    }
    else
        rect.y = -ip.y;

    if (ip.y + win_size.y < src_size.y)
        rect.height = win_size.y;
    else
    {
        rect.height = src_size.y - ip.y - 1;
        if (rect.height < 0)
        {
            src += rect.height * src_step;
            rect.height = 0;
        }
    }

    *pRect = rect;

    return src - rect.x * pix_size;
}

void icvGetRectSubPix_8u32f_C1R_tuned(const u8* src, int src_step,
                                      ClSizeW src_size,
                                      u8* dst,
                                      int dst_step, ClSizeW win_size,
                                      mvCvPoint2D32fW rect_center,
                                      u8 already_padded)
{
    mvCvPointW ip;
    mvCvPoint2D32fW center;
    int j, i;

    center = rect_center;

    center.x -= (win_size.x - 1) * 0.5f;
    center.y -= (win_size.y - 1) * 0.5f;

    ip.x = mvcvFloor(center.x);
    ip.y = mvcvFloor(center.y);

    if (win_size.x <= 0 || win_size.y <= 0)
        return ;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);
    u8 local_src_buff[24 * 24]; // (win_size.x + 1) * (win_size.y + 1), but each line rounded to 8 chars
    u8* src0 = local_src_buff;
    if ((already_padded != 0) ||
        (0 <= ip.x && ip.x + win_size.x < src_size.x &&
        0 <= ip.y && ip.y + win_size.y < src_size.y))
    {
        //Extracted rectangle is completely inside the image
        src += ip.y * src_step + ip.x;
        ClSize aux;
        mvCvPoint2D32f auxf;
        aux.x = win_size.x;
        aux.y = win_size.y;
        auxf.x = rect_center.x;
        auxf.y = rect_center.y;
        if (win_size.x >= 8 && win_size.y >= 5)
        {
            mvcvBilinearInterpolation16u_asm(src, src_step, dst,
                                             dst_step, aux, auxf);
        }
        else
        {
            mvcvBilinearInterpolation16u(src, src_step, dst, dst_step, aux,
                                         auxf);
        }

    }
    //The corner coordinates should take into account the restriction above
    else
    {
        mvCvRect r;
        u32 srcStep = ((win_size.x + 1) &  ~(8-1) ) + 8;
        src = (const u8*) icvAdjustRect_tuned(src, src_step * sizeof(*src),
                                              sizeof(*src),
                                              src_size, win_size, ip, &r);
        smartCopy(src0, (u8*) src, win_size.x + 1);

        for (j = 0; j < r.x; j++)
        {
            src0[j] = src0[r.x];
        }

        for (j = r.width + 1; j < win_size.x; j++)
        {
            src0[j] = src0[r.width];
        }

        i = 1;
        for (; i <= r.y; i++)
        {
            memcpy(src0 + srcStep, src0, win_size.x + 1);
            src0 += srcStep;
        }
        src0 += srcStep;

        for (; i <= r.height; i++)
        {

            // Read the next line only if we don't need padding. Read the same line otherwise
            src += src_step;
            smartCopy(src0, (u8*) src, win_size.x + 1);

            for (j = 0; j < r.x; j++)
            {
                src0[j] = src0[r.x];
            }

            for (j = r.width + 1; j < win_size.x + 1; j++)
            {
                src0[j] = src0[r.width];
            }
            src0 += srcStep;
        }
        for (; i < win_size.y + 1; i++)
        {
            memcpy(src0, src0 - srcStep, win_size.x + 1);
            src0 += srcStep;
        }
        ClSize aux;
        mvCvPoint2D32f auxf;
        aux.x = win_size.x;
        aux.y = win_size.y;
        auxf.x = rect_center.x;
        auxf.y = rect_center.y;

        if (win_size.x >= 8 && win_size.y >= 5)
        {
            mvcvBilinearInterpolation16u_asm(local_src_buff, srcStep,
                                             dst, dst_step, aux, auxf);
        }
        else
        {
            mvcvBilinearInterpolation16u(local_src_buff, srcStep, dst,
                                         dst_step, aux, auxf);
        }

    }
    return ;
}
