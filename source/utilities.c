#include <nds.h>

int stringLength(const char *text) {
    int text_length = 0;
    while(text[text_length] != '\0') {
        text_length++;
    }
    return text_length;
}

void delay (u32 sleep_time) {
    u32 i = 0;
    while (i < sleep_time) {
        swiWaitForVBlank();
        i++;
    }
}
