#ifndef __AVR_INTERRUPT_H__
#define __AVR_INTERRUPT_H__

#ifdef __cplusplus
#define SIGNAL(handler) extern "C" void handler(void)
#else
#define SIGNAL(handler) void handler(void)
#endif

#endif

