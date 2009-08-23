#ifndef __PGMSPACE_H__
#define __PGMSPACE_H__

// Cortex-M3 targets does not use the AVR progmem
// We use const to try to put data in flash, perhaps linker sections can also be used

#define PROGMEM const
#define pgm_read_byte(X)  *(X)

#endif
