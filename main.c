#include <stdbool.h>
#include <stdint.h>

#define RCGCGPIO_R *(volatile uint32_t*) 0x400FE608 // SYSCTL clock register for port F
#define GPIODIR_F_R *(volatile uint32_t*) 0x4005D400 // IO direction pin for port F
#define GPIOAFSEL_F_R *(volatile uint32_t*) 0x4005D420 // GPIO Alternate function select pin
#define GPIODR2R_F_R *(volatile uint32_t*) 0x4005D500 // GPIO 2-mA drive select pin
#define GPIODEN_F_R *(volatile uint32_t*) 0x4005D51C // GPIO digital enable register
#define GPIODATA_F_R *(volatile uint32_t*) 0x4005D008 // Data register, with bits [9:2] = 0b00000010, enabling access to PF1. Write to make it blink!
#define SYSCTL_GPIOHBCTL_R *(volatile uint32_t *) 0x400FE06C // AHB enable register (APB by default)

#define LED_ON() (GPIODATA_F_R = 0x02) // Set PF1 high 
#define LED_OFF() (GPIODATA_F_R = 0x00) // Set PF1 low 

// Wait for a few cycles. * Not a precise way of delaying *
void delay(volatile uint32_t count)
{
    while (count-- > 0) // Make 'count' inclusive with the postfix op
    {
        __asm(" NOP");
    }
}

// Initialize the red LED by configuring PF1 for digital output, etc
void initLED(void)
{
    RCGCGPIO_R |= 0x20; // Enable the clock on port F
    volatile uint32_t dummy = RCGCGPIO_R; // dummy read to make sure the port is enabled before accessing its registers

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
        delay(1000000); // wait
        LED_OFF();
        delay(1000000); // wait
    }
}
