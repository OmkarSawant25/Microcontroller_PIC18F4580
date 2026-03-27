/*
 * File: main.c
 * Author: Omkar Ashok Sawant
 * Description: This Embedded C program generates a growing LED pattern using 8 LEDs connected to PORTB of the PIC18F4580 microcontroller. 
 * Initially, one LED glows and shifts towards the left. 
 * After reaching the end, the number of glowing LEDs increases (1 LED ? 2 LEDs ? 3 LEDs ?). 
 * This continues until all LEDs are ON (11111111). 
 * After that, the pattern resets and starts again. A software delay is used to make the LED movement visible.
 */

#include <xc.h>

#pragma config MCLRE = OFF   // Disable master clear reset pin

int i = 0;

void main(void) {
    ADCON1 = 0x0F; // Configure all pins as digital

    TRISB = 0x00; // Set PORTB as output for LEDs
    PORTB = 0x00; // Initially turn OFF all LEDs

    int x = 1; // Starting LED pattern (00000001)

    unsigned long int delay = 0; // Variable for delay

    while (1) {
        // Create delay for visible LED movement
        if (delay++ == 80000) {
            delay = 0;

            // Shift pattern across LEDs
            if (i < 8) {
                PORTB = x << i;
            } else {
                // Increase number of LEDs ON
                i = -1;
                x |= x << 1;
            }

            // Reset pattern after all LEDs are ON
            if (PORTB == 0xFF) {
                x = 1;
            }

            i++; // Move to next LED position
        }
    }
}