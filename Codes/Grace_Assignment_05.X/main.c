/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 30 March, 2026, 1:36 PM
 */


#include <xc.h>

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

// Key Detection 
#define LEVEL 0
#define EDGE 1

// Switch Mapping 
#define SW1 0x0E   // Increment
#define SW2 0x0D   // Decrement
#define SW3 0x0B   // Choose Field
#define SW4 0x07   // Set/Edit

int once = 1;
unsigned int count = 0;

int minute = 0;
int second = 30;
int field = 0; // 0 = Min , 1 = Sec
char mode = 0; // 0 = Run Mode , 1 = Edit Mode

char read_switch(char detect);
void display(char *ssd);
void __interrupt() isr();

void main(void) {


    TRISD = 0x00;
    TRISA &= 0xF0;

    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    char ssd[4];
    char key;

    // Interrupt enable
    GIE = 1;
    PEIE = 1;
    TMR2IF = 0;
    TMR2IE = 1;

    // Timer2 configuration 
    T2CKPS0 = 0;
    T2CKPS1 = 0;
    PR2 = 249;
    TMR2 = 0;

    TMR2ON = 1;

    while (1) {
        key = read_switch(EDGE);

        if (key == SW4) {
            mode = !mode;
        }

        // Run Mode
        if (mode == 0) {
            if (count < 10000)
                ssd[1] = digit[minute % 10] | 0x10;
            else
                ssd[1] = digit[minute % 10];

            ssd[0] = digit[minute / 10];
            ssd[2] = digit[second / 10];
            ssd[3] = digit[second % 10];

            display(ssd);

            //            if (second == 0) {
            //                minute--;
            //
            //                if (minute >= 0) {
            //                    second = 59;
            //                } else {
            //                    second = 0;
            //                    minute = 0;
            //                }
            //            }

        } else // Edit Mode
        {
            if (key == SW1) {
                if (field) {
                    minute++;
                    if (minute > 99) {
                        minute = 0;
                    }
                } else {
                    second++;
                    if (second > 59) {
                        second = 0;
                    }
                }
            } else if (key == SW2) {
                if (field) {
                    minute--;
                    if (minute < 0) {
                        minute = 99;
                    }
                } else {
                    second--;
                    if (second < 0) {
                        second = 59;
                    }
                }
            } else if (key == SW3) {
                field = !field;
            }

            // Field Blinking 
            if (field == 0) {
                if (count < 10000) {
                    ssd[2] = digit[second / 10];
                    ssd[3] = digit[second % 10];
                } else {
                    ssd[2] = SPACE;
                    ssd[3] = SPACE;
                }

                ssd[0] = digit[minute / 10];
                ssd[1] = digit[minute % 10];

            } else {
                if (count < 10000) {
                    ssd[0] = digit[minute / 10];
                    ssd[1] = digit[minute % 10];

                } else {
                    ssd[0] = SPACE;
                    ssd[1] = SPACE;
                }

                ssd[2] = digit[second / 10];
                ssd[3] = digit[second % 10];

            }
            display(ssd);

        }
    }
    return;
}

// Keypad Read Function 

char read_switch(char detect) {
    if (detect == LEVEL)
        return PORTC & 0x0F;

    else if (detect == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return PORTC & 0x0F;
        } else if ((PORTC & 0x0F) == 0x0F)
            once = 1;
    }
    return 0x0F;
}

// SSD Multiplex Display

void display(char *ssd) {
    for (int i = 0; i < 4; i++) {
        PORTD = ssd[i]; // Send segment data
        PORTA = (PORTA & 0xF0) | (1 << i); // Enable digit
        for (int i = 0; i < 1000; i++);
    }
}

void __interrupt() isr() {
    if (TMR2IF) {
        TMR2IF = 0;

        count++;
        if (count == 20000) { // 1 second
            count = 0;

            if (mode == 0) { // Run mode
                if (second > 0) {
                    second--;
                } else {
                    if (minute > 0) {
                        minute--;
                        second = 59;
                    } else {
                        // Timer reached 00:00
                        minute = 0;
                        second = 0;
                    }
                }
            }
        }
    }
}
