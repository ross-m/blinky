# Project Journal

## Project Title:
`Blinky`

## Overview:
- **Purpose:** `Blink the red LED in a loop. Familiarize myself with reading datasheets, baremetal programming, and basic microcontroller architecture.`
- **Hardware:** TM4C123GXL evaluation board equipped with a TM4C123GH6MPI 32-bit ARM microcontroller unit
- **Tools/Skills:** `CCS`, `C progamming`, `Baremetal programming`, `Register manipulation`, `Reading datasheets`, `GPIO peripherals` 

---

## Daily Log:

---

### 📅 Date: 2025-05-10

**Commit Message:**  
`Configured the PF1 pin for digital GPIO output`

**Goal for the Day:**  
- `Get the pin ready for blinking, figure out how to read the datasheet + what needs to be configured.`

**Actions Taken:**  
- `Followed instructions in section 10.3 of the datasheet to configure pin PF1 (red LED)`

**Technical Details:**  
- `Enable the system clock for port F using the system clock gating control register (RCGCGPIO)`
- `Configure PF1 as an output using the GPIO direction register (GPIODIR)`
- `Disable the alternate function for PF1 using the GPIO alternate function select register (GPIOAFSEL)`
- `Set the drive strength of PF1 to 2-mA using the 2-mA drive select register (GPIODR2R)`
- `Configure PF1 for digital signals using the digital enable register (GPIODEN)`


**Outcome:**  
- `Next steps are to test the configuration by doing some blinking`

**Reflection:**

I've learned that my board is comprised of an MCU (microcontroller unit) that interacts with onboard peripherals through pins. 
The behavior of these pins is controlled with memory-mapped 32-bit registers.

For my blinky project, we're focused on the onboard LED peripheral. It is controlled by GPIO pin PF1. GPIO (general-purpose input/output)
pins are flexible pins that can be re-used for different functions. In our case it's utilized as a digital output pin. Writing to it
tells the LED to turn on and off.


---
