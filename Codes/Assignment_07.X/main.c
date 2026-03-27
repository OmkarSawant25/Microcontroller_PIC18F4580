/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * Program: Time separator using Timer0, Timer1 and Timer2
 * Each timer toggles LED at ~0.5 Hz
 */

#include <xc.h>

void __interrupt() _ISR();

static unsigned int count_0 = 0;
static unsigned int count_1 = 0;
static unsigned int count_2 = 0;

void main(void) {
    TRISB = 0x00; // PORTB as output (LEDs)
    PORTB = 0x00;

    // Enable interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    // Timer0
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 1;

    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS0 = 1;

    TMR0 = 6;
    T0CONbits.TMR0ON = 1;

    // Timer1 
    TMR1IE = 1;
    TMR1IF = 0;
    TMR1 = 3036;
    T1CONbits.TMR1ON = 1;

    // Timer2 
    TMR2IE = 1;
    TMR2IF = 0;
    PR2 = 249;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;

    while (1);

    return;
}

void __interrupt() _ISR() {

    // Timer0 interrupt
    if (INTCONbits.TMR0IF) {
        TMR0 = TMR0 + 8;
        INTCONbits.TMR0IF = 0;

        if (count_0++ == 10000) { // delay generation
            count_0 = 0;
            PORTBbits.RB0 = !PORTBbits.RB0; // toggle LED
        }
    }

    // Timer1 interrupt
    if (TMR1IF) {
        TMR1 = TMR1 + 3038;
        TMR1IF = 0;

        if (count_1++ == 40) { // delay generation
            count_1 = 0;
            PORTBbits.RB1 = !PORTBbits.RB1; // toggle LED
        }
    }

    // Timer2 interrupt
    if (TMR2IF) {
        TMR2IF = 0;

        if (count_2++ == 10000) { // delay generation
            count_2 = 0;
            PORTBbits.RB2 = !PORTBbits.RB2; // toggle LED
        }
    }
}