///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <stdio.h>
#include <registersMyriad.h>
#include "mv_types.h"
#include "app_config.h"
#include "mvHelpersApi.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <UnitTestApi.h>

using namespace std;

#include <iostream>
#include <string>

class SuperClass {
public:
    const char *where;
    SuperClass(const char *where)
    {
        this->where = where;
        printf("Constructing object on %s, at address 0x%08x!\n", where, (unsigned int)this);
    }
    ~SuperClass()
    {
        printf("Destroying object on %s, at address 0x%08x!\n", where, (unsigned int)this);
    }
    virtual void fun() = 0;
};

class SubClass : public SuperClass {
public:

    SubClass(const char *where) : SuperClass(where)
    {

    }

    virtual void fun() {
        printf("Overridden function (obj on %s), obj @ 0x%08x\n", where, (unsigned int)(this));
    }
};

SubClass glob = SubClass("global");
SubClass glob2 = SubClass("global2");

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

	string myname;
	char testInput[20];
	s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        return sc;
    unitTestInit ();
    memcpy((void*)testInput,(const void*)"Myriad2 String\n",20);
    mvSetReadPointer(testInput);

    cout << "Your name:";
    //cin >> myname;
    getline(cin,myname);
    cout << "Hello "<< myname << "!" << endl;
    {
        SubClass x = SubClass("stack");
        SuperClass * y = &x;

        y->fun();
//
//        y = new SubClass("heap");
//        y->fun();
//
//        char buf[1000];
//
//        y = new (buf)SubClass("placement-stack");
//        y->fun();

        glob.fun();

    }
    printf("After block where object is allocated on stack,\n    (destructor should have been called)\n");
    unitTestFinalReport ();

    return 0;
}
