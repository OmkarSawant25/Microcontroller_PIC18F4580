/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 31 March, 2026, 11:10 AM
 */

#include <xc.h>
#include "clcd.h"

#define _XTAL_FREQ 8000000

#define SW1 0x0E
#define SW2 0x0D
#define SW3 0x0B

#define LEVEL 0
#define EDGE 1

char once = 1;

char read_digit_keypad(char detect_type);
void towardsLeft(char *str);
void towardsRight(char *str);

static void init_config(void) {
    init_clcd();
}

void main(void) {
    init_config();

    char str[17] = "Jai Maharashtra!"; // 16 char LCD
    char key;

    int run = 1; // 1 = running, 0 = stopped
    int direction = 0; // 0 = left, 1 = right

    while (1) {
        key = read_digit_keypad(EDGE);

        if (key == SW1) {
            direction = 0; // Left
            run = 1;
        } else if (key == SW2) {
            direction = 1; // Right
            run = 1;
        } else if (key == SW3) {
            run = !run; // Toggle start/stop
        }

        if (run) {
            if (direction == 0)
                towardsLeft(str);
            else
                towardsRight(str);
        }

        clcd_print(str, LINE1(0));
        __delay_ms(2000); // 0.5 Hz
    }
}

char read_digit_keypad(char detect_type) {
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

void towardsRight(char *str) {
    char last = str[15];

    for (int i = 15; i > 0; i--) {
        str[i] = str[i - 1];
    }

    str[0] = last;
}

void towardsLeft(char *str) {
    char first = str[0];

    for (int i = 0; i < 15; i++) {
        str[i] = str[i + 1];
    }

    str[15] = first;
}