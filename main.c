#include <APP_DEFS.h>
#include <GPIO_PF_DEFS.h>
#include <SYSTICK_DEFS.h>

/* Purpose: Demonstrate LED blinking. Main entry point.
*  Parameters: void
*  Returns: void
*/
int main(void) {
    InitializeOnboardLEDModule();
    InitializeOnboardLED(RED);
    InitializeOnboardLED(BLUE);
    InitializeOnboardLED(GREEN);

    while (1)
    {
        ToggleOnboardLED(RED);
        delayByMs(300);
        ToggleOnboardLED(RED);

        ToggleOnboardLED(BLUE);
        delayByMs(300);
        ToggleOnboardLED(BLUE);

        ToggleOnboardLED(GREEN);
        delayByMs(300);
        ToggleOnboardLED(GREEN);

        delayByMs(500);
    }
}
