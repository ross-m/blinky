#include <stdbool.h>
#include <stdint.h>

// GPIO registers
#define RCGCGPIO_R *(volatile uint32_t*) 0x400FE608 // SYSCTL clock register for port F
#define GPIODIR_F_R *(volatile uint32_t*) 0x4005D400 // IO direction pin for port F
#define GPIOAFSEL_F_R *(volatile uint32_t*) 0x4005D420 // GPIO Alternate function select pin
#define GPIODR2R_F_R *(volatile uint32_t*) 0x4005D500 // GPIO 2-mA drive select pin
#define GPIODEN_F_R *(volatile uint32_t*) 0x4005D51C // GPIO digital enable register
#define GPIODATA_F_R *(volatile uint32_t*) 0x4005D008 // Data register, with bits [9:2] = 0b00000010, enabling access to PF1. Write to make it blink!
#define SYSCTL_GPIOHBCTL_R *(volatile uint32_t*) 0x400FE06C // AHB enable register (APB by default)

// Helpers
#define LED_ON() (GPIODATA_F_R = 0x02) // Set PF1 high 
#define LED_OFF() (GPIODATA_F_R = 0x00) // Set PF1 low 

// SysTick timer setup
#define STCTRL *(volatile uint32_t*) 0xE000E010 // control register for the SysTick timer
#define STRELOAD *(volatile uint32_t*) 0xE000E014 // the reload value. this is how many clock cycles elapse before it is reset
#define STCURRENT *(volatile uint32_t*) 0xE000E018 // the value of the register currently. writes clear the register and the count status bit
#define COUNTMASK 0x00010000 // Access the 16th bit (count bit) of STCTRL
#define CYCLESPMS 16000 // 16,000,000 cycles per second * 0.001 seconds per millisecond
#define MAXCYCLES 0xFFFFFF // number of cycles we can represent with 23 bits

/* Purpose: Block CPU for approximately *ms* milliseconds. Resolution is fuzzy because of the polling: the read is not guaranteed
*            to occur exactly when the wrap does because of the cycles consumed executing the looping instructions. For precise timing
*            needs, use an interrupt.
*  Parameters: ms - the number of milliseconds to wait for. Can't wait more than UINT32_MAX milliseconds because of implementation.
*  Returns: void
*/
void delayByMs(volatile uint32_t ms)
{
    if (ms > UINT32_MAX)
    {
        return;
    }

    volatile uint32_t remainingMs = ms;

    // Wait 1 MS at a time
    STRELOAD = CYCLESPMS;

    // clear the cur value register by writing an arbitrary value
    STCURRENT |= 0x1;

    // configure the counter for non-interrupt mode and 80 MHz clock system clock backing
    STCTRL |= 0x5;

    while (remainingMs > 0)
    {
        if ((STCTRL & COUNTMASK) != 0) // works because the count bit clears on read
        {
            remainingMs--;
        }
    }

    // disable the timer
    STCTRL &= ~(0x7);
}

/* Purpose: Initialize the red LED.
*  Parameters: void
*  Returns: void
*/
void initLED(void)
{
    RCGCGPIO_R |= 0x20; // Enable the clock on port F
    delayByMs(1); // definitely consuming at least 3 cycles here

    SYSCTL_GPIOHBCTL_R |= 0x20; // Enable AHB for Port F
    GPIODIR_F_R |= 0x02; // Set LED pin to output mode
    GPIOAFSEL_F_R &= ~(0x02); // Disable the alternate function for this pin
    GPIODR2R_F_R |= 0x02; // Set the current to 2 mA
    GPIODEN_F_R |= 0x02; // Configure pin in digital mode (since we're sending discrete voltages for blinking)
}

/* Purpose: Initialize and blink the LED. Main entry point.
*  Parameters: void
*  Returns: void
*/
int main(void) {
    initLED();

    while (1)
    {
        LED_ON();
        delayByMs(1000);
        LED_OFF();
        delayByMs(1000);
    }
}
