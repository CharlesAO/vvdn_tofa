#include <stdlib.h>
#include <stdio.h>
#include "Shape.h"
//#include "Geometry.h"

//#include <stdint.h>


Shape::Shape(void){
	printf("Shape object created at address, 0x%x\n",reinterpret_cast < unsigned int > (this));
	
}
 
Shape::~Shape(void){
	printf("Shape object: 0x%x destroyed.\n", reinterpret_cast < unsigned int > (this));
}

void Shape::setType(int type){
	this->type = type;

}

int Shape::getType(void){

	return this->type;
}


void Shape::printme(void){
	printf("Hello from Shape\n");

}
