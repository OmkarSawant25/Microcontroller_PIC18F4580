/*
 * File:   main.c
 * Author: Omkar Sawant
 *
 * Title : System Sleep and Wake using External Interrupt (INT0)
 *
 * Functionality:
 *  - On power ON -> Display message "1234" on SSD
 *  - LED toggles every 1 second indicating system is active
 *  - If no interrupt activity for 5 seconds -> System enters SLEEP mode
 *  - On pressing INT0 switch -> System wakes up from sleep
 */

#include <xc.h>

/* SSD Segment Codes for displaying 1 2 3 4 */
#define ONE   0x21
#define TWO   0xCB
#define THREE 0x6B
#define FOUR  0x2D

/* Function Prototypes */
void display(char *ssd);
void __interrupt() _ISR();

/* Global variable to track inactivity time */
unsigned int inactivity_time = 0;

void main(void) {
    /* PORT Configuration */
    TRISB = 0x01; // RB0 as input (INT0), RB7 as LED output
    TRISD = 0x00; // SSD segment lines as output
    TRISA &= 0xF0; // SSD digit enable lines as output

    PORTB = 0x80; // Initially LED OFF (RB7 high)

    /* Message to be displayed */
    char digits[] = {ONE, TWO, THREE, FOUR};
    char ssd[4];

    unsigned int delay = 0;

    /* Interrupt Configuration */
    INTCONbits.GIE = 1; // Enable Global Interrupt
    INTCONbits.PEIE = 1; // Enable Peripheral Interrupt 
    INTCONbits.INT0IE = 1; // Enable External Interrupt INT0
    INTCONbits.INT0IF = 0; // Clear Interrupt Flag

    while (1) {
        /* Load SSD message */
        for (int i = 0; i < 4; i++) {
            ssd[i] = digits[i];
        }

        /* Continuously refresh display */
        display(ssd);

        /* Generate ~1 second delay using software counter */
        if (delay++ == 300) {
            delay = 0;

            /* Toggle activity LED */
            PORTBbits.RB7 = !PORTBbits.RB7;

            /* Increment inactivity counter */
            inactivity_time++;
        }

        /* If no interrupt activity for 5 seconds -> Enter sleep */
        if (inactivity_time == 5) {
            SLEEP(); // MCU enters low power sleep mode
        }
    }
}

/* SSD Multiplex Display Function */
void display(char *ssd) {
    for (int digit = 0; digit < 4; digit++) {
        PORTD = ssd[digit]; // Send segment data
        PORTA = (PORTA & 0xF0) | (1 << digit); // Enable one SSD at a time

        for (int delay = 0; delay < 1000; delay++); // Persistence delay
    }
}

/* Interrupt Service Routine */
void __interrupt() _ISR() {
    /* Check if INT0 interrupt occurred */
    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0; // Clear interrupt flag

        /* Reset inactivity timer -> system becomes active again */
        inactivity_time = 0;
    }
}