///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool allocator
///

#include <MemAllocator.h>
#include <mv_types.h>
#include <assert.h>
#include <stdio.h>

//####################################################################
//Note: even if baseAddr is not aligned to target "alignment" param
//      allocs get done at "page_size" boundary
void RgnAllocator::Create(void *baseAddr, uint32_t sz, uint32_t alignment)
{
   rtems_status_code sc;
 //This will fail if rtems_config doesn't set something like
 //#define CONFIGURE_MAXIMUM_REGIONS 1
   sc =  rtems_region_create(rtems_build_name('R','g','n',' '),
                             baseAddr,
                             sz,
                             alignment, //page_size
                             RTEMS_DEFAULT_ATTRIBUTES,
                             &rgnId);
   if(sc != OK)
       printf("rtems_region_create err %d\n", sc);
   created = 1;
   assert(sc == OK);
};

//####################################################################
void RgnAllocator::Delete()
{
    rtems_status_code sc;
    sc = rtems_region_delete(rgnId);
    assert(sc == OK);
}

//####################################################################
void *RgnAllocator::Alloc(size_t sz)
{
   void  *ptr;
   rtems_status_code sc;
   assert(created);
   sc = rtems_region_get_segment(rgnId, sz, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT, &ptr);
   assert(sc == OK);
   return ptr;
};

//####################################################################
void RgnAllocator::Free(void *ptr){
   rtems_status_code sc;
   assert(created);
   sc = rtems_region_return_segment(rgnId, ptr);
   assert(sc == OK);
};

//####################################################################
//Stock objects
//####################################################################
uint8_t       RgnBuff[DEF_POOL_SZ] ALIGNED(32) SECTION(DEF_POOL_SECT);
uint8_t       RgnBuffCMX[CMX_POOL_SZ] ALIGNED(32) SECTION(CMX_POOL_SECT);
RgnAllocator  RgnAlloc;
RgnAllocator  RgnAllocCMX;
HeapAllocator HeapAlloc;
