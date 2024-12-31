#include <reg51.h>

#define SEGMENT_PORT P2
#define CONTROL_PORT P3

uint8_t code segment_data[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Delay(uint32_t ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++){
    	for (j = 0; j < 0x2aff; j++);
	}
}

void LED7_Write(uint8_t tens, uint8_t units) {
    CONTROL_PORT = 0xFE;
    SEGMENT_PORT = segment_data[tens];
    Delay(5);

    CONTROL_PORT = 0xFD;
    SEGMENT_PORT = segment_data[units];
    Delay(5);
}

void main() {
    unsigned char tens, units;
    unsigned char number;

    while (1) {
        for (number = 10; number <= 70; number++) {
            tens = number / 10;
            units = number % 10;
            for (int i = 0; i < 200; i++) {
                display_number(tens, units);
            }
        }
    }
}
