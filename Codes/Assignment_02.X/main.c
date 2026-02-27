/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 25 February, 2026, 4:42 PM
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
int i = 0;

char read_switch(char detect_type);
void pattern_1();
void pattern_2();

void main(void) {
    TRISB = 0x00;
    TRISC |= 0x0F;

    PORTB = 0x00;

    unsigned long int delay = 0;
    int flag = 0;

    while (1) {
        key = read_switch(LEVEL);
        if (key == SW1) {
            i = 0;
            flag = 1;
        }
        if (key == SW2) {
            i = 0;
            flag = 2;
        }
        if (key == SW3) {
            PORTB = 0XAA;
            flag = 3;
        }
        if (key == SW4) {
            PORTB = 0XF0;
            flag = 4;
        }
        if (delay++ == 80000) {
            delay = 0;
            if (flag == 1) {
                // Pattern 1
                pattern_1();
            } else if (flag == 2) {
                // Pattern 2
                pattern_2();

            } else if (flag == 3 || flag == 4) {
                // Pattern 3 || Pattern 4
                PORTB = ~PORTB;
            }
        }
    }
    return;
}

char read_switch(char detect_type) {
    if (detect_type == LEVEL) {
        return PORTC & 0x0F;
    } else if (detect_type == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return PORTC & 0x0F;
        } else if ((PORTC & 0x0F) == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}

void pattern_1() {
    if (i < 8) {
        PORTB = PORTB | (1 << i);
    } else if (i >= 8 && i < 16) {
        PORTB = PORTB << 1;
    } else if (i >= 16 && i < 24) {
        PORTB = PORTB | (0X80) >> (i - 16);
    } else if (i >= 24 && i < 32) {
        PORTB >>= 1;
    } else {
        i = -1;
    }
    i++;
}

void pattern_2() {
    if (i < 8) {
        PORTB = PORTB | (1 << i);
    } else if (i >= 8 && i < 16) {
        PORTB = PORTB << 1;
    } else {
        i = -1;
    }
    i++;
}