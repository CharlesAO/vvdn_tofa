#include <bilinearInterpolation.h>

typedef struct {
    u16 factorAA, factorBB, factorCC, factorDD;
} lookUpT;

void bilinearInterpolation(
                    u8 * line1,
                    u8 * line2,
                    u8 * output,
                    u16 * precalcPos,
                    u16 * lut,
                    int stopW,
                    int godValue)
{
    lookUpT *lutt = (lookUpT*)lut;

    u16 precalcPosCpy[10];
    for (int i = 0; i < 10; i++)
    {
        precalcPosCpy[i] = precalcPos[i];
    }

    for (int j = 0; j < stopW / 10; j++)
    {
        for (int k = 0; k < 10; k++)
        {
            u16 nX = precalcPosCpy[k];
            precalcPosCpy[k] += godValue;

            *output++ =
                   ( line1[nX]     * lutt[k].factorAA
                   + line1[nX + 1] * lutt[k].factorBB
                   + line2[nX]     * lutt[k].factorCC
                   + line2[nX + 1] * lutt[k].factorDD) >> 8;
        }
    }

    for (int j = 0; j < stopW % 10; j++)
    {
        u16 nX = precalcPosCpy[j];

        *output++ =
               ( line1[nX]     * lutt[j].factorAA
               + line1[nX + 1] * lutt[j].factorBB
               + line2[nX]     * lutt[j].factorCC
               + line2[nX + 1] * lutt[j].factorDD) >> 8;
    }
}