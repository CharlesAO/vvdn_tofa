#include "mv_types.h"
#include "PixelPipeApi.h"

void UpdateCellThresholds(fp32* thresholds, tvFeatureCell* featureCells,
                          ppThresholds_t* thresholdCfg, u32 nCells, u32 targetNumFeatures)
{
    u32 i;
    fp32 newCellThreshold;
    u32 targetFeaturesPerCell = targetNumFeatures/nCells;
    for (i = 0; i < nCells; i++)
    {
        if (featureCells[i].featureCount < targetFeaturesPerCell)
        {
            newCellThreshold = thresholds[i]
                            * thresholdCfg->thresholdDecreaseVelocity;
            if (newCellThreshold < thresholdCfg->thresholdMin)
                newCellThreshold = thresholdCfg->thresholdMin;
        }
        else
        {
            newCellThreshold = thresholds[i]
                            * thresholdCfg->thresholdIncreaseVelocity;
            if (newCellThreshold > thresholdCfg->thresholdMax)
                newCellThreshold = thresholdCfg->thresholdMax;
        }
        thresholds[i] = newCellThreshold;
    }
}



