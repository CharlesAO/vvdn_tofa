// file to provide stubs for cross porting Eigen

#include <stdio.h>
#include <stdlib.h>
#include <stubs.h>
#include <time.h>

time_t time(time_t *t)
{
  return *t;
}


char* arrayu="String to pass to Eigen";
int pos=1;
int __srget_r(struct _reent * rent, FILE * stream)
{
  if ((rent!=NULL) || (stream !=NULL))
     return arrayu[pos++];
  else
     return 0;
}


int _Z6test_1v(void)
{
return 0;
}


int _Z5test_v(void)
{
return 0;
}


int _Z9__setheapPvj(void)
{
    return 0;
}

/*
int ungetc(int c, FILE *name)
{
  if (name != NULL)
   return c;
  else
   return 0;
}


int fflush(FILE *stream)
{
  if (stream==NULL){
  return 0;
  }else{
   return 1;
  }
}

*/
