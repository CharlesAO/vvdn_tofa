//#include <stdint.h>

#ifndef _GEOMETRY_H_

#define _GEOMETRY_H_
#include "Shape.h"


class Geometry: public Shape{
   public:
      Geometry();
      ~Geometry(); 
      unsigned int somefun(void);
      void printme(void);
  
};


#endif
