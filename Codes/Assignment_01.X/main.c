/*
 * File: main.c
 * Author: Omkar Ashok Sawant
 * Program: LED Train Pattern using PIC18F4580
 */

#include <xc.h>

void main(void) {
    TRISB = 0x00; // Set PORTB as output for LEDs
    PORTB = 0x00; // Initially turn OFF all LEDs

    unsigned int delay = 0; // Variable for delay
    int i = 0; // Variable to control LED pattern

    while (1) {
        // Generate delay for visible LED movement
        if (delay++ == 50000) {
            delay = 0;

            // Turn ON LEDs from Left to Right
            if (i < 8) {
                PORTB |= (1 << i);
            }
            // Turn OFF LEDs from Left to Right
            else if (i < 16) {
                PORTB = PORTB << 1;
            }
            // Turn ON LEDs from Right to Left
            else if (i < 24) {
                PORTB |= (0x80 >> (i - 16));
            }
            // Turn OFF LEDs from Right to Left
            else if (i < 32) {
                PORTB = PORTB >> 1;
            }
            // Restart pattern
            else {
                i = -1;
            }

            i++; // Move to next step
        }
    }

    return;
}