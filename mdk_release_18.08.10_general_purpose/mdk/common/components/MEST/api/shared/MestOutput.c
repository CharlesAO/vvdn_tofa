#include <mvMacros.h>

#include <MestPublicTypes.h>
#include <MestOutput.h>

void mestParseOutput(void *buffer, mestResults_t *result)
{
    mestOutput_t *res = (mestOutput_t*) buffer;
    const uint32_t size = res->header.outputFlowCnt;
    result->nrOfVectors = size;

    buffer += ALIGN_UP(sizeof(mestOutput_t), 8);
    result->meta.outputFlows = (xyPos_t*) buffer;
    buffer += ALIGN_UP(size * sizeof(xyPos_t), 8);
    result->meta.costs = (uint16_t*) buffer;
}
