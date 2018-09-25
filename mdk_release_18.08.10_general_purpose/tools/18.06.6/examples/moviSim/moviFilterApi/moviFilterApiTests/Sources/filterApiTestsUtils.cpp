#include <stdio.h>
#include <stdlib.h>

#include "filterApiTestsUtils.h"

void loadMemFromFile(char *fileName, uint32_t length, void* buffer)
{
    FILE *f = fopen(fileName, "rb");
    if (f != NULL)
    {
        fread(buffer, 1, length, f);
        fclose(f);
    }
    else {
        printf("ERROR file %s not found !", fileName);
    }
}

bool unitTestCrcCheck(void * pStart, uint32_t lengthBytes, uint32_t expectedCrc)
{
    uint8_t *addr = (uint8_t*)pStart;

    const uint32_t CRC32_POLY = 0x04C11DB7;
    uint32_t crc32 = 0xffffffff;
    uint32_t temp = 0;
    uint32_t i;
    uint32_t bitNum;
    for (i = 0; i < lengthBytes; i++)
    {
        uint8_t current_byte = addr[i];
        for (bitNum = 0; bitNum < 8; bitNum++)
        {
            int msb = (crc32 >> 31) & 1;
            crc32 = crc32 << 1;
            if (msb != ((current_byte >> bitNum) & 1))
            {
                crc32 ^= CRC32_POLY;
                crc32 |= 1;
            }
        }
    }
    for (bitNum = 0; bitNum < 32; bitNum++)
    {
        if ((crc32 >> bitNum) & 1)
        {
            temp |= 1 << (31 - bitNum);
        }
    }
    crc32 = ~temp;

    return crc32 == expectedCrc;
}
