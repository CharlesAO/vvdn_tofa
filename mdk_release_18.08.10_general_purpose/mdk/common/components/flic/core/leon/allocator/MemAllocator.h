///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool allocator
///

#ifndef __FLIC_ALLOCATOR_H__
#define __FLIC_ALLOCATOR_H__

#include <Flic.h>

//##############################################################
class RgnAllocator: public IAllocator{
  private:
   uint32_t  created;
   rtems_id  rgnId; //region ID
  public:
  //some of the VideoEncode buffs require 32Byte alignment
   RgnAllocator() {created = 0;};
   void  Create(void *baseAddr, uint32_t sz, uint32_t alignment = 64);
   void  Delete();
   void *Alloc(size_t size);
   void  Free (void *ptr);
};

//##############################################################
class HeapAllocator : public IAllocator{
 private:
   uint32_t _alignment;
 public:
   HeapAllocator(size_t alignment = 64) : _alignment(alignment) {}
   void *Alloc(size_t size) {
        void *ptr;
        int sc = posix_memalign(&ptr, _alignment, size);
        assert(sc == OK);
        return ptr;
   }
   void  Free (void *ptr){
       free(ptr);
   }
};

//##############################################################
//Stock objects
extern uint8_t       RgnBuff[];
extern uint8_t       RgnBuffCMX[];
extern RgnAllocator  RgnAlloc;
extern RgnAllocator  RgnAllocCMX;
extern HeapAllocator HeapAlloc;

#endif
