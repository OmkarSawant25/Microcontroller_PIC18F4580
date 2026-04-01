/*
 * File: main.c
 * Author: Omkar Sawant
 *
 * Description:
 * LED Dimmer using Timer2 ISR (Software PWM).
 * Duty cycle controls LED brightness.
 * SW1 ? Increase brightness
 * SW2 ? Decrease brightness
 */

#include <xc.h>

#define SW1 0x0E
#define SW2 0x0D
#define LEVEL 0
#define EDGE 1

#define _XTAL_FREQ 8000000

char once = 1;
char key;

unsigned int duty_cycle = 50; // Initial 50% brightness
unsigned int count = 0; // PWM counter

char read_switch(int type);
void __interrupt() _ISR(void);

void main(void) {
    TRISB = 0x00; // PORTB as output (LED)
    TRISC |= 0x0F; // PORTC lower bits as input (switch)

    GIE = 1;
    PEIE = 1;
    TMR2IF = 0;
    TMR2IE = 1;

    TMR2 = 0;
    PR2 = 249;
    T2CONbits.TMR2ON = 1;

    while (1) {
        key = read_switch(EDGE);

        if (key == SW1 && duty_cycle < 100)
            duty_cycle++;

        if (key == SW2 && duty_cycle > 0)
            duty_cycle--;
    }
}

char read_switch(int type) {
    if (type == LEVEL)
        return PORTC & 0x0F;

    if ((PORTC & 0x0F) != 0x0F && once) {
        once = 0;
        return PORTC & 0x0F;
    } else if ((PORTC & 0x0F) == 0x0F) {
        once = 1;
    }

    return 0x0F;
}

// Timer2 ISR for PWM

void __interrupt() _ISR(void) {
    if (TMR2IF) {
        TMR2IF = 0;

        if (count < duty_cycle)
            PORTB = 0xFF; // LED ON
        else
            PORTB = 0x00; // LED OFF

        count++;
        if (count >= 100)
            count = 0;
    }
}