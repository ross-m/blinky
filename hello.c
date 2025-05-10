#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"

// PF1 is the red LED. 

#define RCGCGPIO_R *(volatile uint32_t*) 0x400FE608 // SYSCTL clock register for port F
#define GPIODIR_F_R *(volatile uint32_t*) 0x4005D400 // IO direction pin for port F
#define GPIOAFSEL_F_R *(volatile uint32_t*) 0x4005D420 // GPIO Alternate function select pin
#define GPIODR2R_F_R *(volatile uint32_t*) 0x4005D500 // GPIO 2-mA drive select pin
#define GPIODEN_F_R *(volatile uint32_t*) 0x4005D51C

void initLED(void)
{
    RCGCGPIO_R |= (1 << 5); // Enable the clock on port F
    volatile uint32_t dummy;
    dummy = RCGCGPIO_R; // Dummy read to introduce 3-cycle delay (required before accessing the port we just enabled)
    GPIODIR_F_R |= (1 << 1); // Set LED pin to output mode
    GPIOAFSEL_F_R &= ~(1 << 1) // Disable the alternate function for this pin
    GPIODR2R_F_R |= (1 << 1) // Set the current to 2 mA
    GPIODEN_F_R |= (1 << 1) // Configure pin in digital mode (since we're sending discrete voltages for blinking)
}

int main(void) {
    initLED();

    
}

