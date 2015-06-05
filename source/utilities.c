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

int waitKey() {
	int keys = keysCurrent();

	while((keys & 0b111111111111) == 0) {
		keys = keysCurrent();
		delay(1);
	}
	return keys;
}

// waits for the user to press A or B
int waitAB() {
	int keys;
	do {
		keys = waitKey();
	} while(!(keys & KEY_A) && !(keys & KEY_B));
}
