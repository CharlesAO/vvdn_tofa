///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy echo test + RTEMS CPU/workspace stats
///

//#include <rtems/monitor.h>
#include <rtems/libcsupport.h>
#include <rtems/score/heap.h>
#include <rtems/cpuuse.h>
#include <rtems/posix/semaphore.h>
#include <Flic.h>
#include "PlgEcho.h"

PlgEchoSend  plgSend;
PlgEchoRecv  plgRecv;
Pipeline     p;

//#########################################################
//RTEMS workspace usage
//Note: increasing number of resources in rtems_config.c
//      increases workspace !!! (e.g. 1Sem = +328 Bytes)

Heap_Information_block  wi;
rtems_resource_snapshot snap;
extern Heap_Control _Workspace_Area; //cool stuff
extern Heap_Control *RTEMS_Malloc_Heap;//heap

void rtemsInfo()
{
   extern int      end;       //here is where RTEMS workspace starts, concides with "__bss_end"
   extern uint32_t rdb_start;
   extern int _LEON_STACK_POINTER;
  //"uintptr_t work_area_size  = (uintptr_t)rdb_start - (uintptr_t)&end - STACK_SIZE;"
  //rdb_start set in "hard_reset" to point to _LEON_STACK_POINTER, which is ORIGN(LOS) + LENGTH(LOS)
  //Thead stack is actually mapped somewhere in workspace area, the .stack section in default ldscript
  //is not a real stack section from RTEMS perspective !!!

   printf("================================================\n");
   printf("RTEMS mem starts @ 0x%lx ; ends @ 0x%lx\n", (uint32_t)&end, rdb_start);

   rtems_workspace_get_information(&wi);
   printf("FREE total = %u (num = %u)\n", wi.Free.total, wi.Free.number);
   printf("USED total = %u (num = %u)\n", wi.Used.total, wi.Used.number);

   rtems_resource_snapshot_take(&snap);

   printf("[WorkSpace]\n");
   printf("  .area_begin  @ 0x%x\n", (unsigned int)_Workspace_Area.area_begin);
   printf("  .area_end    @ 0x%x\n", (unsigned int)_Workspace_Area.area_end  );

   printf("[Heap]\n");
   printf(" RTEMS_Malloc_Heap->area_begin  @ 0x%lx\n", (uint32_t)RTEMS_Malloc_Heap->area_begin);
   printf(" RTEMS_Malloc_Heap->area_end    @ 0x%lx\n", (uint32_t)RTEMS_Malloc_Heap->area_end  );
   printf("_LEON_STACK_POINTER             @ 0x%lx\n", (uint32_t)&_LEON_STACK_POINTER );//stack = unfortunate name
   //heap end is actually _LEON_STACK_POINTER - 8KB, as per work_area_size calculation above

   printf(" Heap SIZE = %d\n", RTEMS_Malloc_Heap->stats.size);
   printf(" Heap FREE = %d\n", RTEMS_Malloc_Heap->stats.free_size);
   printf("================================================\n");
   sleep(8);
}

//############################################
extern "C" void *POSIX_Init (void *)
{
    #if defined(CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY)
        printf("CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY\n"); //NOT defined
    #endif

    printf("Sem size = %d\n", sizeof(      Semaphore_Control));
    printf("Sem size = %d\n", sizeof(POSIX_Semaphore_Control));


    /*DBG*/ rtemsInfo();

    plgSend.Create();
    plgRecv.Create();

    p.Add(&plgSend);
    p.Add(&plgRecv);

    plgSend.out.Link(&plgRecv.in);

    p.Start();


  //===================================================
  //name threads so it looks nice on log !!!
    rtems_interrupt_level level;
    rtems_status_code     status;

    rtems_interrupt_disable( level ); //this is a MACRO that changes "level"
      status = rtems_object_set_name(pthread_self(), "POSIX_Init");  //printf("Status=%d\n", status);
      status = rtems_object_set_name(plgSend.thread, "plgSend_thr"); //printf("Status=%d\n", status);
      status = rtems_object_set_name(plgRecv.thread, "plgRecv_thr"); //printf("Status=%d\n", status);
      UNUSED(status);
    rtems_interrupt_enable( level );
  //===================================================

  {
   rtems_interrupt_level level;
   rtems_id id = plgSend.thread;
   rtems_interrupt_disable( level ); //this is a MACRO that changes "level"
      printf("\n\n");
      printf("sizeof(pthread_t) = %d\n", sizeof(pthread_t));
      printf("id                        %lx\n", id);
      printf("rtems_object_id_get_api   %lx\n", (uint32_t)rtems_object_id_get_api  (id));
      printf("rtems_object_id_get_class %lx\n", (uint32_t)rtems_object_id_get_class(id));
      printf("rtems_object_id_get_node  %lx\n", (uint32_t)rtems_object_id_get_node (id));
      printf("rtems_object_id_get_index %lx\n", (uint32_t)rtems_object_id_get_index(id));
   rtems_interrupt_enable( level );
  }


  //cool stuff: cpu: usage
    while(1){
        sleep(1);
        rtems_cpu_usage_report();
    }

    p.Wait();
    exit(0);
}
