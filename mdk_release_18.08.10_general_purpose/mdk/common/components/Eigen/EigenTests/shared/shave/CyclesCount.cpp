#include <stdlib.h>
#include "CyclesCount.h"

// this function returns number of CPU cycles elapsed .

typedef unsigned int u32;
unsigned int swcReadPC3(void)
{
      u32 local_var_result; \
        asm ( \
            "lsu0.lda.32 %[result], 4, 0x184c"     "\n\t" \
            "NOP 6" \
            : [result] "=r" (local_var_result) \
            : \
            : "memory" \
        ); \
        return local_var_result;
}
