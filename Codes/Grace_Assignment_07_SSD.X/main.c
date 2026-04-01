/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 31 March, 2026, 11:10 AM
 */

#include <xc.h>

#define _XTAL_FREQ 8000000

#define SW1 0x0E
#define SW2 0x0D
#define SW3 0x0B

// SSD Digit Codes 
#define ZERO 0xE7
#define ONE 0x21
#define TWO 0xCB
#define THREE 0x6B
#define FOUR 0x2D
#define FIVE 0x6E
#define SIX 0xEE
#define SEVEN 0x23
#define EIGHT 0xEF
#define NINE 0x6F
#define SPACE 0x00

#define LEVEL 0
#define EDGE 1

char once = 1;

char read_digit_keypad(char detect_type);
void towardsLeft(int *str);
void towardsRight(int *str);
void init_ssd();
void display(char *ssd);

void main(void) {
    init_ssd();
    char key;

    int run = 1; // 1 = running, 0 = stopped
    int direction = 0; // 0 = left, 1 = right

    int str[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10};
    int digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, SPACE};
    char ssd[4];

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

        ssd[0] = digit[str[0]];
        ssd[1] = digit[str[1]];
        ssd[2] = digit[str[2]];
        ssd[3] = digit[str[3]];

        for (int i = 0; i < 500; i++) {
            display(ssd);
        }
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

void towardsRight(int *str) {
    int last = str[11];

    for (int i = 11; i > 0; i--) {
        str[i] = str[i - 1];
    }

    str[0] = last;
}

void towardsLeft(int *str) {
    int first = str[0];

    for (int i = 0; i < 11; i++) {
        str[i] = str[i + 1];
    }

    str[11] = first;
}

void init_ssd() {
    TRISD = 0X00;
    TRISA = TRISA & 0XF0;
    PORTD = 0X00;
}

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data];
        PORTA = (PORTA & 0XF0) | (1 << data);
        for (int delay = 1000; delay--;);
    }
}
