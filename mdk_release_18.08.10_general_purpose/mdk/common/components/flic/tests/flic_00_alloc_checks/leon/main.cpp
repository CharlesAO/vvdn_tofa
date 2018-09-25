///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     - override new/delete
///            - check constructor/destructor calls
///            - check mem leak
///

#include <Flic.h>
#include <UnitTestApi.h>
#include <MemCheck.h>

#define N_RUNS 100

int *iPtr[N_RUNS];

//#########################################
#define INIT_1 44
#define INIT_2 55

class MyClass
{
  public:
    uint32_t data;
    static int nDestroy;
    MyClass() {data = INIT_1;}
    virtual ~MyClass() {nDestroy++;}
};
//=========================================
class MyClass2 : public MyClass
{
  public:
    uint32_t buff[16];
    static int nDestroy;
    MyClass2() {buff[0] = INIT_2;}
    virtual ~MyClass2() {nDestroy++;}
};

int MyClass ::nDestroy = 0;
int MyClass2::nDestroy = 0;

typedef struct {
  uint32_t data1;
  uint16_t data2;
  uint8_t  data3;
} MyData ALIGNED(16);

//#########################################
extern "C" void *POSIX_Init (void *args)
{
    uint32_t i;
    UNUSED(args);
    unitTestInit();

    printf("__alignof__(MyClass) : %d\n", __alignof__(MyClass));
    printf("__alignof__(MyData)  : %d\n", __alignof__(MyData));
    printf("     sizeof(MyData)  : %d\n",      sizeof(MyData));

    int *ioio = new int;
    delete ioio;

    for(i=0; i<N_RUNS; i++)
    {
       int *ii = new int [128];
       UNUSED(ii);
       delete[] ii;
    }

    for(i=0; i<N_RUNS; i++) iPtr[i] = new int [8];
    for(i=0; i<N_RUNS; i++) delete[] iPtr[i];

  //===================================================
    unitTestAssert(MyClass::nDestroy == 0);
    for(i=0; i<N_RUNS; i++){
      MyClass *obj = new MyClass();
      unitTestAssert(obj->data == INIT_1);
      delete obj;
    }
    unitTestAssert(MyClass::nDestroy == N_RUNS);

  //===================================================
    unitTestAssert(MyClass2::nDestroy == 0);
    for(i=0; i<N_RUNS; i++){
      MyClass2 *obj = new MyClass2();
      unitTestAssert(obj->buff[0] == INIT_2);
      delete obj;
    }
    unitTestAssert(MyClass ::nDestroy == N_RUNS*2);
    unitTestAssert(MyClass2::nDestroy == N_RUNS);

    printf("THE END.\n");

    ioio = new int;
    unitTestAssert(OK != CheckMemLeaks(1));

    unitTestFinalReport();
    exit(0);
}