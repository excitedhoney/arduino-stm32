//*****************************************************************************
//
// startup_cm3.c - Startup code for use with GNU tools.
//
// Startup and default exception handlers Cortex-M3 targets 
//
//*****************************************************************************

__attribute__ ((section(".startup")))

//*****************************************************************************
//
// Linker sybols
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss_start;
extern unsigned long _bss_end;

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Initialisation of global objects.
//
//*****************************************************************************
extern void __libc_init_array (void);

//*****************************************************************************
//
//  Copy data segment from FLASH to RAM and clear bss segment
//  No stach setup is necessary for Cortex-M3 
//
//*****************************************************************************
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

void ResetISR(void)
{
    register unsigned long *pulSrc, *pulDest;
    int count;
    int i;


    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    for(pulDest = &_bss_start; pulDest < &_bss_end; )
    {
        *pulDest++ = 0;
    }

    //
    // Call constructors for global objects, adresses to  initialisation routins are in __init_array
    //
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i] ();

    main();
}

void NmiSR(void)
{
    // Enter an infinite loop.
    while(1)
    {
    }
}

void FaultISR(void)
{
    // Enter an infinite loop.
    while(1)
    {
    }
}

void IntDefaultHandler(void)
{
    // Go into an infinite loop.
    while(1)
    {
    }
}

void IntUnhandledHandler(void)
{
    // Return  to program
}

