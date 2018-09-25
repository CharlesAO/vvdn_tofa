#ifndef __AGGREGATE_FIVE_PATHS_64_ASM_TEST__
#define __AGGREGATE_FIVE_PATHS_64_ASM_TEST__


extern unsigned int mvcvAggregateFivePaths64CycleCount;
void mvcvAggregateFivePaths64_asm_test(unsigned char *path0, unsigned char *path1, 
                                       unsigned char *path2, unsigned char *path3, 
                                       unsigned char *path4, unsigned char *out,
                                       unsigned int width);

#endif //__AGGREGATE_FIVE_PATHS_64_ASM_TEST__
