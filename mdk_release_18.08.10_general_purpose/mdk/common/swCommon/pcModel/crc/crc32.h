#include "stdint.h"

uint32_t calcCrc(uint8_t *buffer, uint32_t size);
uint32_t contCalcCrc(uint8_t *buffer, uint32_t size, uint32_t prevValue);