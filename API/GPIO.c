#include <APP_DEFS.h>
#include <GPIO_PF_DEFS.h>

/* 
*  Purpose: Initialize the chosen onboard LED by applying the following configurations to the appropriate GPIO pin:
*    - Setting it to output mode
*    - Disabling the alternate function
*    - Setting the current to 2 mA
*    - Configuring it in digital mode
*  Parameters: 
*    - led: Which onboard LED to configure. Can be red, green, or blue.
*  Returns: uint8_t corresponding to success / failure. Failure occurs when an unrecognized LED is given.
*  Assumes: Port F has sys clock enabled and is using AHB
*/
uint8_t InitializeOnboardLED(LED_pin led){
    uint32_t offset = 0x0;

    switch (led) 
    {
        case RED:
            offset = 0x02;
            break;
        case BLUE:
            offset = 0x04;
            break;
        case GREEN:
            offset = 0x08;
            break;
        default:
            return FAILURE;
    }

    GPIODIR_F_R |= offset; // Set LED pin to output mode
    GPIOAFSEL_F_R &= ~(offset); // Disable the alternate function for this pin
    GPIODR2R_F_R |= offset; // Set the current to 2 mA
    GPIODEN_F_R |= offset; // Configure pin in digital mode (since we're sending discrete voltages for blinking)

    return SUCCESS;
}

/* 
*  Purpose: Toggle the given LED. ON -> call() -> OFF. OFF -> call() -> ON.
*  Parameters: 
*    - led: Which onboard LED to configure. Can be red, green, or blue.
*  Returns: uint8_t corresponding to success / failure. Failure occurs when an unrecognized LED is given.
*  Assumes: Port F has sys clock enabled and is using AHB, and *led* has been initialized appropriately.
*/
uint8_t ToggleOnboardLED(LED_pin led)
{
    switch (led) 
    {
        case RED:
            GPIODATA_PF1_R ^= 0x02;
            break;
        case BLUE:
            GPIODATA_PF2_R ^= 0x04;
            break;
        case GREEN:
            GPIODATA_PF3_R ^= 0x08;
            break;
        default:
            return FAILURE;
    }

    return SUCCESS;
}

/* 
*  Purpose: Enable the system clock for GPIO port F, which is where the onboard LED pins live. AHB mode is used.
*  Parameters: none
*  Returns: none
*  Assumes: nothing
*/
void InitializeOnboardLEDModule()
{
    RCGCGPIO_R |= 0x20; // Enable sys clock
    delayByMs(1); // wait at least 3 cycles
    SYSCTL_GPIOHBCTL_R |= 0x20; // Enable AHB 
}
