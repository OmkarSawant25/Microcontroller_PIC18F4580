/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 26 February, 2026, 3:47 PM
 */


#include <xc.h>

#define SW1 0x0E
#define SW2 0x0D
#define SW3 0x0B
#define SW4 0x07
#define LEVEL 0
#define EDGE 1

char key;
char once = 1;

char read_switch(char detect_type);

void main(void) {
    TRISB = 0x00;
    TRISC |= 0x0F;

    PORTB = 0X00;
    unsigned long int delay = 0;

    while (1) {
        key = read_switch(LEVEL);
        if (delay++ == 100000) {
            delay = 0;

            if (key == SW1) {
                // one led
                PORTB = 0X01;
            } else if (key == SW2) {
                // two led
                PORTB = 0X03;
            } else if (key == SW3) {
                // Alternate LED
                PORTB = 0XAA;
            } else if (key == SW4) {
                // ALL TURN on
                PORTB = 0XFF;
            } else {
                PORTB = 0X00;
            }
        }
    }
    return;
}

char read_switch(char detect_type) {
    if (detect_type == LEVEL)
        return PORTC & 0x0F;
    else if (detect_type == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return PORTC & 0x0F;
        } else if ((PORTC & 0x0F) == 0x0F)
            once = 1;
    }
    return 0x0F;
}