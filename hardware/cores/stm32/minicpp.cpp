#include <stdlib.h>
/* Minimal c++ support for embedded systems*/

/* Non throwing version of operator new */ 
void *operator new(size_t size) throw() {return malloc(size);}

/* Non throwing version of operator delete */ 
void operator delete(void *p) throw() {free(p);}

extern "C" void __cxa_pure_virtual()
{
  while (1);
}

