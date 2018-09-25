#ifndef __NULL_ALLOCATOR__
#define __NULL_ALLOCATOR__

#define PHONY_ADDR 0xDEADBEEF

//Just return some dummy value, no alloc or free
//To be used with preallocated static buffers for test mostly
class NullAllocator : public IAllocator
{
 public:
   void *Alloc(size_t size) { (void)size; return (uint8_t*)PHONY_ADDR; }
   void  Free (void  *ptr ) { (void)ptr;                               }
};

#endif