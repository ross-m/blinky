#ifndef SYS_DEFS
#define SYS_DEFS

#include <stdint.h>
#include <stdbool.h>

#define SUCCESS (uint8_t)0 // error code for success
#define FAILURE (uint8_t)-1 // error code for failure

void delayByMs(uint32_t ms); // Block for this many milliseconds

#endif
