#include <stdlib.h>
#include <stdio.h>
#include "Geometry.h"


// class Geometry: public Shape{
//    public:
//       Geometry();
//        ~Geometry(); 
//       unsigned int somefun(void);
  
// };



// #ifdef __cplusplus
// extern "C" {
// #endif
// void*  	createGeometry();
// void  	destroyGeometry(void *foo);
// unsigned int somefunc(void* foo);
// #ifdef __cplusplus
// }
// #endif



Geometry::Geometry(void){
	printf("Geometry object created at address, 0x%x\n",
	       reinterpret_cast < unsigned int > (this));
	//std::cout<<"Geometry created.\n";
}
 
Geometry::~Geometry(void){
	printf("Geometry object: 0x%x destroyed.\n", reinterpret_cast < unsigned int > (this));
}

unsigned int Geometry::somefun(void){
	return 42;
}


void Geometry::printme(void){
	printf("Hello from Geometry\n");

}


// void* createGeometry(){
// 	//printf("createGeometry called.\n");
//    	Geometry *out = new Geometry();
//    	return ((void*)out);
// }

// void destroyGeometry(void* foo){
// //	printf("destroyGeometry called.\n");
//    	delete (((Geometry*)foo));
// }

// unsigned int somefunc(void* foo){
// 	int i;
// 	i = ((Geometry*)foo)->somefun();
// 	return i;
   
// }
