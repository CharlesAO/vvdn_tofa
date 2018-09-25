///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "app_config.h"
#include "Geometry.h"
#include "Shape.h"
#include <UnitTestApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

Geometry globalobj;
Geometry *dynglobalobj;

int main(void)
{
    s32 sc;

    sc = initClocksAndMemory();
    if(sc)
        return sc;
    unitTestInit ();    
    printf("\n\nHello from LEON!\n\n\n");

    printf("Creating obj statically...\n");
    Geometry obj;
    int id2 = obj.somefun();
    printf("obj somefun returned: %d\n", id2);

    printf("Creating dynobj dynamically...\n");
    Geometry *dynobj = new Geometry();
   	int id=  dynobj->somefun();
   	printf("dynobj somefun returned: %d\n", id);

   	printf("Accessing inherited method: of dynobj\n");
   	dynobj->setType(6);
   	int type = dynobj->getType();
   	printf("Dynobj shape type: %d\n", type);

   	printf("Testing method overriding\n");
   	Shape *shape = new Geometry();
   	Geometry *geom = new Geometry();

   	shape->printme();
   	geom->printme();

    printf("Printme from global...\n");
    globalobj.printme();
    printf("Printme from global dynamically...\n");
    dynglobalobj = new Geometry();
    dynglobalobj->printme();

   	printf("Destroying objects\n");
   	delete dynobj;
   	delete shape;
   	delete geom;
   	printf("Destroying Static objects\n");
   	printf("....................\n");
    unitTestFinalReport ();
    return 0;
}
