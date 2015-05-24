#include <nds.h>

void delay (u32 sleep_time) {
    u32 i = 0;
    do {
        swiWaitForVBlank();
        i++;
    } while (i < sleep_time);
}
