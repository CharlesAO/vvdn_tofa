#ifndef MEST_OUTPUT_H
#define MEST_OUTPUT_H

#include <stdint.h>

typedef struct xyPos_t
{
    float x;
    float y;
} xyPos_t;

typedef struct
{
    xyPos_t *outputFlows;
    uint16_t *costs;
} mestMeta_t;

typedef struct
{
    void *address;
    uint32_t size;
} mestBulkOutput_t;

typedef struct
{
    uint32_t nrOfVectors;
    mestMeta_t meta;
} mestResults_t;

#ifdef __cplusplus
extern "C" {
#endif

void mestParseOutput(void *buffer, mestResults_t *result);

#ifdef __cplusplus
}
#endif

#endif // MEST_OUTPUT_H
