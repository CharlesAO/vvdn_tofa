#include "sampleApp.h"

void sampleApp(unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out)
{
  unsigned int i;
  short aux;

  switch(operation)
  {
  case 0:
      // subtraction
      for(i = 0; i < pxToChange; i++)
      {
          aux = (short)in[i] - (short)value;
          if( aux < 0)
              aux = 0;
          out[i] = (unsigned char) aux;
      }
      break;
  case 1:
      // addition
      for(i = 0; i < pxToChange; i++)
      {
          aux = (short)in[i] + (short)value;
          if ( aux > 255)
              aux = 255;
          out[i] = (unsigned char) aux;
      }
      break;
  default: break;
  }
  
  for(i = pxToChange; i <  width; i++)
      out[i] = in[i];
}
