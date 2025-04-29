#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"

#define RCGCGPIO_R *(volatile uint32_t*) 0x400FE608 // SYSCTL clock register for port F
#define GPIODIR_F_R *(volatile uint32_t*) 0x4005D400 // IO direction pin for port F

void initLED(void)
{
    RCGCGPIO_R |= (1 << 5); // Enable the clock on port F
    volatile uint32_t dummy;
    dummy = RCGCGPIO_R; // Dummy read to introduce 3-cycle delay (required before accessing the port we just enabled)
    GPIODIR_F_R |= (1 << 1); // Set LED pin to output mode
}

int main(void) {
    initLED();
}

