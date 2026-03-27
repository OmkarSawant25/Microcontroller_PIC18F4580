/*
 * File: main.c
 * Author: Omkar Ashok Sawant
 */

#include <xc.h>

#define _XTAL_FREQ 8000000

// Segment codes for digits 0?9
#define ZERO  0xE7
#define ONE   0x21
#define TWO   0xCB
#define THREE 0x6B
#define FOUR  0x2D
#define FIVE  0x6E
#define SIX   0xEE
#define SEVEN 0x23
#define EIGHT 0xEF
#define NINE  0x6F

void display(char *ssd); // Function to display digits

void main(void) {
    TRISD = 0x00; // PORTD as output (segment data)
    TRISA &= 0xF0; // Lower 4 bits of PORTA as output (digit select)

    PORTA &= 0xF0; // Initially disable all digits

    // Array storing segment codes for digits
    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

    char ssd[4]; // Array for 4-digit display

    while (1) {
        // Load digits to display (0 1 2 3)
        ssd[0] = digit[0];
        ssd[1] = digit[1];
        ssd[2] = digit[2];
        ssd[3] = digit[3];

        // Display digits using multiplexing
        display(ssd);
    }

    return;
}

// Function to multiplex and display 4 digits

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data]; // Send segment data
        PORTA = (PORTA & 0xF0) | (1 << data); // Enable one digit

        for (int delay = 1000; delay--;); // Small delay for visibility
    }
}