/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 30 March, 2026, 11:53 AM
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
#define DOT 0x10

#define _XTAL_FREQ 8000000

int hour = 12;
int min = 0;
int sec = 0;
unsigned int count = 0;

void __interrupt() _ISR(void);
void display(char *ssd);

void main(void) {

    TRISD = 0X00;
    TRISA = TRISA & 0XF0;

    GIE = 1;
    PEIE = 1;
    TMR2IF = 0;
    TMR2IE = 1;

    PR2 = 249;
    TMR2 = 0;
    TMR2ON = 1;

    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    char ssd[4];

    while (1) {

        if (count < 10000) {
            ssd[1] = digit[hour % 10] | DOT;
        } else
            ssd[1] = digit[hour % 10];

        ssd[0] = digit[hour / 10];
        ssd[2] = digit[min / 10];
        ssd[3] = digit[min % 10];

        display(ssd);

        if (sec == 60) {
            sec = 0;
            min++;
            if (min == 60) {
                min = 0;
                hour++;
                if (hour == 24) {
                    min = 0;
                    hour = 0;
                }
            }
        }
    }



    return;
}

void __interrupt()ISR() {
    if (TMR2IF) {
        TMR2IF = 0;
        if (count++ == 20000) {
            count = 0;
            sec++;
        }
    }
}

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data];
        PORTA = (PORTA & 0XF0) | (1 << data);
        __delay_ms(2);
    }
}
