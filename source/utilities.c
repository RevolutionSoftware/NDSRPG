#include <nds.h>

void delay (u32 sleep_time) {
    u32 i = 0;
    while (i < sleep_time) {
        swiWaitForVBlank();
        i++;
    }
}
