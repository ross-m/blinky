#ifndef GPIO_REG
#define GPIO_REG

#define RCGCGPIO_R *(volatile uint32_t*) 0x400FE608 // SYSCTL clock register for port F
#define GPIODIR_F_R *(volatile uint32_t*) 0x4005D400 // IO direction pin for port F
#define GPIOAFSEL_F_R *(volatile uint32_t*) 0x4005D420 // GPIO Alternate function select pin
#define GPIODR2R_F_R *(volatile uint32_t*) 0x4005D500 // GPIO 2-mA drive select pin
#define GPIODEN_F_R *(volatile uint32_t*) 0x4005D51C // GPIO digital enable register
#define GPIODATA_PF1_R *(volatile uint32_t*) 0x4005D008 // Data register, with bits [9:2] = 0b00000010, enabling access to PF1. Write to make it blink!
#define GPIODATA_PF2_R *(volatile uint32_t*) 0x4005D010 // Data register, with bits [9:2] = 0b00000100, enabling access to PF2. Write to make it blink!
#define GPIODATA_PF3_R *(volatile uint32_t*) 0x4005D020 // Data register, with bits [9:2] = 0b00001000, enabling access to PF3. Write to make it blink!
#define SYSCTL_GPIOHBCTL_R *(volatile uint32_t*) 0x400FE06C // AHB enable register (APB by default)

typedef enum {
    RED = 0,
    BLUE = 1,
    GREEN = 3,
} LED_pin;

void InitializeOnboardLEDModule(); // Abstract wrapper around GPIO port F. We'll call it a 'module' and say it must be enabled before its LEDs are touched.
uint8_t InitializeOnboardLED(LED_pin led); // Setup the pin corresponding to the passed LED for blinking.
uint8_t ToggleOnboardLED(LED_pin led); // Flip the LED 

#endif
