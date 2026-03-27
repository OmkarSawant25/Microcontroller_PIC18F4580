/*
 * File:   BIT_BANGING.c
 * Author: OMKAR SAWANT
 *
 * Created on 25 March, 2026, 10:58 AM
 */


#include <xc.h>
#define _XTAL_FREQ 20000000

#define LEVEL 0
#define EDGE 1
#define PERIOD 1000
#define SW1 0x0E
#define SW2 0x0D

char key;
unsigned int once = 1;
unsigned int duty_cycle = 0;
unsigned int delay = 0;

void config();
char read_switch(char detect_type);

void main(void) {
    config();
    while (1) {
        key = read_switch(EDGE);
        if (key == SW1) {
            if (duty_cycle < PERIOD) {
                duty_cycle++;
                __delay_ms(50);
            }
        } else if (key == SW2) {
            if (duty_cycle > 0) {
                duty_cycle--;
                __delay_ms(50);
            }
        }

        if (delay < duty_cycle) {
            RB0 = 1;
        } else {
            RB0 = 0;
        }

        if (delay++ == PERIOD) {
            delay = 0;
        }
    }
    return;
}

void config() {
    TRISB0 = 0;
    RB0 = 0;
    TRISC = TRISC | 0x0F;
}

char read_switch(char detect_type) {
    if (detect_type == LEVEL) {
        return (PORTC & 0x0F);
    } else if (detect_type == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return (PORTC & 0x0F);
        } else if ((PORTC & 0x0F) == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}