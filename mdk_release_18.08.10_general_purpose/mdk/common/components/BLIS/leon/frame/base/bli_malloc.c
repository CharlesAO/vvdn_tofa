/*

   BLIS    
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "blis.h"
#include "mv_types.h"
//#define LEON_MALLOC_ISSUE
#ifdef LEON_MALLOC_ISSUE
#include "string.h"

typedef struct memory_header_t memory_header;
struct memory_header_t
{
   size_t          size;
   size_t          alloc;
   memory_header   *prev;
   memory_header   *next;
   memory_header   *prev_free;
   memory_header   *next_free;
};

typedef struct
{
   unsigned char   *buf;
   size_t          len;
   memory_header   *first;
   memory_header   *first_free;
   size_t          current_alloc_size;
} buffer_alloc_ctx;

static buffer_alloc_ctx heap;

static void *bli_malloc_leon(size_t len)
{
   memory_header *new, *cur = heap.first_free;
   unsigned char *p;

   if(heap.buf == NULL || heap.first == NULL)
   {
      return(NULL);
   }

   if(len % BLIS_HEAP_ADDR_ALIGN_SIZE)
   {
      len -= len % BLIS_HEAP_ADDR_ALIGN_SIZE;
      len += BLIS_HEAP_ADDR_ALIGN_SIZE;
   }

   /* Find block that fits */
   while(cur != NULL)
   {
      if(cur->size >= len)
      {
         break;
      }

      cur = cur->next_free;
   }

   if(cur == NULL)
   {
      return(NULL);
   }

   if(cur->alloc != 0)
   {
      printf("Error bli_malloc_leon (cur->alloc != 0)\n");
      exit(1);
   }

   /* Found location, split block if > memory_header + 4 room left */
   if(cur->size - len < sizeof(memory_header) + BLIS_HEAP_ADDR_ALIGN_SIZE)
   {
      cur->alloc = 1;

      /* Remove from free_list */
      if( cur->prev_free != NULL )
      {
         cur->prev_free->next_free = cur->next_free;
      }
      else
      {
         heap.first_free = cur->next_free;
      }

      if( cur->next_free != NULL )
      {
         cur->next_free->prev_free = cur->prev_free;
      }

      cur->prev_free = NULL;
      cur->next_free = NULL;

      return ((unsigned char *)cur) + sizeof(memory_header);
   }

   p = ((unsigned char *)cur) + sizeof(memory_header) + len;
   new = (memory_header *)p;

   new->size = cur->size - len - sizeof(memory_header);
   new->alloc = 0;
   new->prev = cur;
   new->next = cur->next;

   if(new->next != NULL)
   {
      new->next->prev = new;
   }

   /* Replace cur with new in free_list */
   new->prev_free = cur->prev_free;
   new->next_free = cur->next_free;
   if(new->prev_free != NULL)
   {
      new->prev_free->next_free = new;
   }
   else
   {
      heap.first_free = new;
   }

   if(new->next_free != NULL)
   {
      new->next_free->prev_free = new;
   }

   cur->alloc = 1;
   cur->size = len;
   cur->next = new;
   cur->prev_free = NULL;
   cur->next_free = NULL;

   return ((unsigned char *)cur) + sizeof(memory_header);
}

static void bli_free_leon(void *ptr)
{
   memory_header *hdr, *old = NULL;
   unsigned char *p = (unsigned char *) ptr;

   if(ptr == NULL || heap.buf == NULL || heap.first == NULL)
   {
      return;
   }

   if(p < heap.buf || p > heap.buf + heap.len)
   {
      printf("error bli_free_leon (p < heap.buf || p > heap.buf + heap.len)\n");
      exit( 1 );
   }

   p -= sizeof(memory_header);
   hdr = (memory_header*)p;

   if(hdr->alloc != 1)
   {
      printf("error bli_free_leon (hdr->alloc != 1)\n");
      exit( 1 );
   }

   hdr->alloc = 0;

   /* Regroup with block before */
   if(hdr->prev != NULL && hdr->prev->alloc == 0)
   {
      hdr->prev->size += sizeof(memory_header) + hdr->size;
      hdr->prev->next = hdr->next;
      old = hdr;
      hdr = hdr->prev;

      if(hdr->next != NULL)
      {
         hdr->next->prev = hdr;
      }

      memset(old, 0, sizeof(memory_header));
   }

   /* Regroup with block after */
   if(hdr->next != NULL && hdr->next->alloc == 0)
   {
      hdr->size += sizeof(memory_header) + hdr->next->size;
      old = hdr->next;
      hdr->next = hdr->next->next;

      if(hdr->prev_free != NULL || hdr->next_free != NULL)
      {
         if(hdr->prev_free != NULL)
         {
            hdr->prev_free->next_free = hdr->next_free;
         }
         else
         {
            heap.first_free = hdr->next_free;
         }

         if(hdr->next_free != NULL)
         {
            hdr->next_free->prev_free = hdr->prev_free;
         }
      }

      hdr->prev_free = old->prev_free;
      hdr->next_free = old->next_free;

      if(hdr->prev_free != NULL)
      {
         hdr->prev_free->next_free = hdr;
      }
      else
      {
         heap.first_free = hdr;
      }

      if(hdr->next_free != NULL)
      {
         hdr->next_free->prev_free = hdr;
      }

      if(hdr->next != NULL)
      {
         hdr->next->prev = hdr;
      }

      memset(old, 0, sizeof(memory_header));
   }

   /* Prepend to free_list if we have not merged */
   /* (Does not have to stay in same order as prev / next list) */
   if(old == NULL)
   {
      hdr->next_free = heap.first_free;
      heap.first_free->prev_free = hdr;
      heap.first_free = hdr;
   }
}

int bli_init_leon( unsigned char *buf, siz_t len)
{
    memset(&heap, 0, sizeof(buffer_alloc_ctx) );
#if 0
    memset(buf, 0, len );
#else
    memset(buf, 0, 256);
#endif

    heap.buf = buf;
    heap.len = len;

    heap.first = (memory_header *) buf;
    heap.first->size = len - sizeof(memory_header);
    heap.first_free = heap.first;
    return(0);
}

void bli_deinit_leon()
{
    memset(&heap, 0, sizeof(buffer_alloc_ctx));
}
#endif /*LEON_MALLOC_ISSUE*/

void* bli_malloc( siz_t size )
{
    void* p = NULL;
	int   r_val;
	UNUSED(r_val);

	if ( size == 0 ) return NULL;

#ifdef LEON_MALLOC_ISSUE
	p = bli_malloc_leon( ( size_t )size );
#else
#if BLIS_HEAP_ADDR_ALIGN_SIZE == 1
	p = malloc( ( size_t )size );
#else
	r_val = posix_memalign( &p,
	                        ( size_t )BLIS_HEAP_ADDR_ALIGN_SIZE,
	                        ( size_t )size );

	if ( r_val != 0 ) bli_abort();
#endif
#endif

   if(p == NULL)
   {
      printf("Error allocation %lld\n", (u64)size);
      bli_abort();
   }

	return p;
}

void bli_free( void* p )
{
#ifdef LEON_MALLOC_ISSUE
   bli_free_leon(p);
#else
	free( p );
#endif
}

