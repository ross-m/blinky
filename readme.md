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

### Date: 2025-05-10

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


### Date: 2025-05-12

**Goal for the Day:**  
- `Finish configuring pin PF1`
- `Get the pin blinking and process what was wrong with my initial setup`

**Actions Taken:**  
- `Debugged my initialization by referencing the datasheet and re-inspecting my setup`

**Technical Details:**  
- `Read the datasheet example of a GPIODATA write to correctly define the address mask for PF1`
- `Learned the difference between the AHB and APB memory apertures, and how to toggle between them with the GPIOHBCTL register`

**Outcome:**  
- `Succesfully started blinking the LED`

**Next steps:**  
- `Refactor the code`
- `Identify a better way to introduce delays when it's critical (such as waiting for the port's clock to be enabled)`

**Reflection:**

I made two mistakes in my initial setup. The first was not understanding the masking behavior described in section 
10.2.1.2 of the datasheet. I didn't read the fine print of the GPIODATA description at first and just used 0x4005D000
as my reference to the register. When the blinking didn't happen, I skimmed from the beginning of the GPIO intro section
and stumbled across the example. After reading the example there, I realized that I would need to access address
0x4005D000 (such that the 2nd bit is 1) to enable PF1. The explanation for why was interesting: the read-modify-write 
problem with concurrency is something that comes up in the distributed computing space. It's discussed a lot in the
context of database consistency levels. In any case, I changed my approach and began addressing the register correctly.

My second mistake was not enabling the AHB memory aperture. I saw that there were two different base addresses for each port, 
and I recalled from the setup steps in section 10.3 of the datasheet that there were two 'memory apertures' to access the peripherals:
APB (legacy) and AHB (high performance). These 'memory apertures' provide different physical addresses to access the same peripherals with, and I figured based on 
the description that I should use the more modern, high performance one. but I did not realize at first that it had to be explicitly enabled in the GPIOHBCTL register.
After re-reading that section during debugging, I realized my mistake.

My overall takeway is that the datasheet is the embedded programmer's bible. Everything I needed was in there,
I just needed to slow down and pay closer attention to the details.

### Date: 2025-05-13

**Next steps:**  
- `Feeling a little stumped. On my next commit, I will hopefully be able to explain why. Right now the LED is stuck on.`

### Date: 2025-05-14

**Goal for the Day:**  
- `Fix my timer code`

**Actions Taken:**  
- `Uncommented the lines that blinked the LED`

**Technical Details:**  
- `Read the datasheet to infer the default clock rate`

**Outcome:**  
- `Succesfully started blinking the LED`

**Next steps:**  
- `Refactor the code`
- `Make a cleaner delay function that measures time rather than cycle count`

**Reflection:**

Unfortunately, I was frantically trying different things to fix my code last time, so my full path to discovery cannot
by put on display for you. Commenting out the last 3 lines was an attempt to see if my timer interval was somehow very
small: I thought I might detect a subtle difference between "ON" and "ON and OFF really fast" with my eyeballs. When I stepped
into the debugger with fresh eyes today, I immediately saw my mistake.

When I ctrl-Z'd to see what I did before, I found that I had been passing too large of a value to the delay function. I was mistakenly
passing 160,000,000, but I explicitly reject values that exceed the maximum of the STRELOAD register (2^24 - 1, or 16,777,216). I read in 
section 5.2.5.1 that the Precision Internal Oscillator is the default system clock after Power-on Reset (PON), and that it runs at 16 MHz, so I was aiming for that. I discovered that my edge condition checking was working - thereby breaking the delay logic - by placing a breakpoint on my second call to delay (in between the blinks) and using CCS's debugger.

---
