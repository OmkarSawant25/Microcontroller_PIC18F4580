/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Program: Toggle PORTB using Timer0 interrupt
 * Created on 10 March, 2026, 4:50 PM
 */

#include <xc.h>

int count = 0; // Counter to create longer delay

void __interrupt() _ISR(void);

void main(void) {
    TRISB = 0x00; // Configure PORTB as output
    PORTB = 0X00; // Initially clear PORTB

    // Enable Interrupts 
    INTCONbits.GIE = 1; // Global Interrupt Enable
    INTCONbits.PEIE = 1; // Peripheral Interrupt Enable
    INTCONbits.TMR0IE = 1; // Enable Timer0 interrupt

    // Timer0 Configuration 
    T0CONbits.T08BIT = 1; // Timer0 in 8-bit mode
    T0CONbits.T0CS = 0; // Clock source = Internal clock (Fosc/4)
    T0CONbits.PSA = 0; // Enable prescaler for Timer0

    // Prescaler value = 1:4 
    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS0 = 1;

    TMR0 = 6; // Initial preload value for Timer0
    T0CONbits.TMR0ON = 1; // Start Timer0

    while (1); // Infinite loop (all work done in ISR)

    return;
}

// Interrupt Service Routine 

void __interrupt()_ISR(void) {
    if (INTCONbits.TMR0IF) // Check if Timer0 overflow interrupt occurred
    {
        TMR0 = TMR0 + 8; // Reload timer for next interrupt
        INTCONbits.TMR0IF = 0; // Clear Timer0 interrupt flag

        // Count multiple interrupts to generate larger delay 
        if (count++ == 5000) {
            count = 0; // Reset counter

            PORTB = !PORTB; // Toggle PORTB (LED ON/OFF)
        }
    }
}