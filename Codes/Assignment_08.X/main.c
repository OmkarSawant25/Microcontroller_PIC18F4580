/*
 * File:   main.c
 * Author: Omkar Sawant
 * 
 * Program: Dynamic Clock on SSD using Internal Timer (24 Hr Format)
 * Starts from 00.00
 * Run Mode  -> Clock runs normally
 * Edit Mode -> User can set Hour / Minute
 * DP Blinks every 500 ms
 */

#include <xc.h>

#define _XTAL_FREQ 8000000
#pragma config MCLRE = OFF

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

// Time Variables 
int count = 0; // Used for 500ms blink + 1 sec generation
int hours = 0;
int minute = 0;
int second = 0;

int once = 1;

// Timer0 Interrupt -> Generates 1 second 
void __interrupt() _ISR() {
    if (INTCONbits.TMR0IF) {
        TMR0 = TMR0 + 8; // Reload timer
        INTCONbits.TMR0IF = 0;

        if (count++ == 5000) // After required overflows -> 1 second
        {
            count = 0;
            second++; // Increase seconds
        }
    }
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
        __delay_ms(2);
    }
}

void main(void) {
    int field = 1; // 1 = Minute field selected
    char mode = 0; // 0 = Run Mode , 1 = Edit Mode
    char key;

    // Port Configuration 
    TRISD = 0x00;
    TRISA &= 0xF0;
    TRISC |= 0x0F;

    PORTD = 0x00;

    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    char ssd[4];

    // Timer0 Interrupt Enable 
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    // Timer0 Config 
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS0 = 1;

    TMR0 = 6;
    T0CONbits.TMR0ON = 1;

    while (1) {
        key = read_switch(EDGE);

        // Toggle Run/Edit Mode
        if (key == SW4) {
            if (mode == 1)
                second = 0;

            mode = !mode;
        }

        // RUN MODE 
        if (mode == 0) {
            // DP blinking every 500ms 
            if (count < 2500)
                ssd[1] = digit[hours % 10] | 0x10;
            else
                ssd[1] = digit[hours % 10];

            ssd[0] = digit[hours / 10];
            ssd[2] = digit[minute / 10];
            ssd[3] = digit[minute % 10];

            display(ssd);

            // Time Update 
            if (second == 60) {
                minute++;
                second = 0;

                if (minute == 60) {
                    hours++;
                    minute = 0;

                    if (hours == 24)
                        hours = 0;
                }
            }
        }
            // EDIT MODE 
        else {
            if (key == SW3)
                field = !field;

            if (key == SW1) {
                if (field)
                    minute = (minute + 1) % 60;
                else
                    hours = (hours + 1) % 24;
            }

            if (key == SW2) {
                if (field)
                    minute = (minute == 0) ? 59 : minute - 1;
                else
                    hours = (hours == 0) ? 23 : hours - 1;
            }

            // Field Blinking 
            if (field) {
                if (count < 2500) {
                    ssd[2] = digit[minute / 10];
                    ssd[3] = digit[minute % 10];
                } else {
                    ssd[2] = SPACE;
                    ssd[3] = SPACE;
                }

                ssd[0] = digit[hours / 10];
                ssd[1] = digit[hours % 10];
            } else {
                if (count < 2500) {
                    ssd[0] = digit[hours / 10];
                    ssd[1] = digit[hours % 10];
                } else {
                    ssd[0] = SPACE;
                    ssd[1] = SPACE;
                }

                ssd[2] = digit[minute / 10];
                ssd[3] = digit[minute % 10];
            }

            display(ssd);
        }
    }
}