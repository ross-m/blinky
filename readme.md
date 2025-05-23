# Project Journal

## Project Title: `Blinky`

## Overview:
- **Purpose:** `Blink the onboard LEDs without TI's HAL libraries. Familiarize myself with reading datasheets, baremetal programming, and basic microcontroller architecture.`
- **Hardware:** `TM4C123GXL evaluation board equipped with a TM4C123GH6MPI 32-bit ARM microcontroller unit`
- **Tools/Skills:** `CCS`, `C progamming`, `Baremetal programming`, `Register manipulation`, `Reading datasheets`, `GPIO peripherals` 
- **References:**
  - [TM4C123GH6PM datasheet](https://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf)
  - [TM4C123GXL evaluation board user's guide](https://www.ti.com/lit/ug/spmu296/spmu296.pdf)

Note: I cleaned up the repository to exclude some autogenerated Code Composer Studio files that build / configure the project. I did this to make it easy to identify the
code I wrote myself. One exception is the `tm4c123gh6pm_startup_csc.c` file, which was provided by TI and has a dependency on a function called `_c_int00`
which I have yet to identify the source of. These files setup the interrupt vector table and call into my main method. In the future I'd like to write this
myself.
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

### Date: 2025-05-14

**Goal for the Day:**  
- `Change my timer function to be time-based instead of clock-based`

**Actions Taken:**  
- `Re-structured the timer function`
- `Changed documentation structure of functions`

**Technical Details:**  
- `Polled the count bit on STCTRL register`
- `Measured time using the SysTick timer`

**Outcome:**  
- `Success`

**Next steps:**  
- `Reorganize everything`
- `Maybe add button debouncing to learn interrupts`

**Reflection:**

I realized it didn't make very much sense to measure real-world stuff with clock cycles. That was useful when I needed to wait precisely
three cycles for port F to have its clock enabled, but for blinking the LED, time is a more appropriate measure. It wasn't too difficult
to do after writing the first version. I knew that I could determine when a millisecond had elapsed by loading a millisecond's worth of ticks into
STRELOAD, and that I could count how many times that happened with a looping variable. 

However, I also realized that this was an imprecise way of measuring time. The count bit could be set while we're in the middle of executing arbitrary
CPU instructions for the looping code, so we're unlikely to execute the masking instructions exactly at the moment STCURRENT wraps. This was okay for 
my purposes because the delay would be imperceptably small in the context of blinking an LED. Since each instruction in the loop probably takes no more than 
a handful of clock cycles, and there are likely no more than a few such instructions, the delay is likely on the order of nanoseconds, ie (1 second / 16,000,000 cycles) = 6.25 e^-10 seconds per cycle * average 5 cycles per instruction * ~100 instructions between reads, ballpark conservative estimate) is on the order
of 10^-9. I could probably get precise figures if I read the assembly and read the documentation for the instruction set, but that's way too deep for what I'm doing. Maybe some day.

### Date: 2025-05-18

**Goal for the Day:**  
- `API-ify and refactor the project`

**Actions Taken:**  
- `Reorganized the file structure`
- `Defined abstract APIs for the onboard LEDs and SysTick timer`

**Technical Details:**  
- `Defined an abstract 'Onboard LED module' which represents GPIO port F`
- `Defined helper methods that allow you to enable and toggle specific onboard LEDs`

**Outcome:**  
- `Access to the onboard LEDs and systick timer is through my new functions`

**Next steps:**  
- `Improve timer usage`
- `Use a custom build toolchain`

**Reflection:**

Now that everything is working, I decided it would be a good exercise in C programming and software development to define APIs for the peripherals I'm working on.
To that end, I've split out the LED and SysTick calls into helper functions and cleaned up register definitions / type defs into their own header files. This includes
adding support for the green and blue LEDs, since my original project only supported red.

My next steps should be to improve the way I'm using the SysTick timer. I don't like polling because it is imprecise and wastes CPU cycles. I plan on doing some 
background research to see what patterns are common.

Additionally, I want to learn how to compile + link + flash the board myself. Right now I'm dependent on Code Composer Studio for that, and I want to do it 
myself at least once to learn those skills.

---
