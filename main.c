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

// Wait at least *cycleCount* cycles. "At least" because there's no way to guarantee the counter
// will be read precisely at the moment it first wraps.
void waitForCycleCount(volatile uint32_t cycleCount)
{
    if (cycleCount >= 0x1000000) // STRELOAD only reserves bits 0-23, so block any value beyond or including 2^24
    {
        return;
    }

    // set the reload value
    STRELOAD = cycleCount - 1; // Timer counts the 0th tick

    // clear the cur value register
    STCURRENT &= 0xFFFFFFFF;

    // configure the STCTRL register for system clock (bit 2, 1), interrupt gen off (bit 1, 0), and timer on (bit 0, 1)
    STCTRL |= 0x5; 

    // Wait until at least a single wrap has occurred (which guarantees AT LEAST *cycleCount* cycles have elapsed)
    while ((STCTRL & COUNTMASK) == 0);

    // clear the cur value register
    STCURRENT &= 0xFFFFFFFF;

    // disable the lowest 3 bits of the STCTRL register, disabling / resetting the timer
    STCTRL &= ~(0x7);
}

// Initialize the red LED by configuring PF1 for digital output, etc
void initLED(void)
{
    RCGCGPIO_R |= 0x20; // Enable the clock on port F
    waitForCycleCount(10); // datasheet says 3 clocks, but lets do 10 to be safe

    SYSCTL_GPIOHBCTL_R |= 0x20; // Enable AHB for Port F
    GPIODIR_F_R |= 0x02; // Set LED pin to output mode
    GPIOAFSEL_F_R &= ~(0x02); // Disable the alternate function for this pin
    GPIODR2R_F_R |= 0x02; // Set the current to 2 mA
    GPIODEN_F_R |= 0x02; // Configure pin in digital mode (since we're sending discrete voltages for blinking)
}

// Initialize and blink the LED
int main(void) {
    initLED();

    while (1)
    {
        LED_ON();
       // waitForCycleCount(16000000); // wait
       // LED_OFF();
       // waitForCycleCount(16000000); // wait
    }
}
