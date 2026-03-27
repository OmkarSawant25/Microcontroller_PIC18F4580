/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * Program: Implement a left scrolling number marquee on 4-digit SSD
 */

#include <xc.h>

// Hex values for displaying digits on Seven Segment Display
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
#define SPACE 0x40     // Blank space on SSD

void display(char *ssd); // Function prototype for SSD display

void main(void) {
    TRISD = 0x00; // Configure PORTD as output 
    TRISA &= 0xF0; // Configure lower 4 bits of PORTA as output (digit selection lines)

    PORTD = 0X00; // Clear PORTD
    PORTA &= 0xF0; // Clear lower 4 bits of PORTA

    // Array containing digits 0?9 followed by two spaces for smooth scrolling
    char digits[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, SPACE, SPACE};

    char ssd[4]; // Array to store currently displayed digits (4 SSDs)
    int index = 0; // Starting index for scrolling

    while (1) {
        // Load 4 consecutive digits from digits[] for scrolling display
        ssd[0] = digits[index % 12];
        ssd[1] = digits[(index + 1) % 12];
        ssd[2] = digits[(index + 2) % 12];
        ssd[3] = digits[(index + 3) % 12];

        // Refresh display multiple times to maintain persistence of vision
        for (int i = 0; i < 100; i++)
            display(ssd);

        index++; // Move to next position (scroll left)

        // Reset index when end of array is reached
        if (index == 12) {
            index = 0;
        }
    }

    return;
}

// Displays 4 digits on multiplexed seven segment display.

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data]; // Send segment data to PORTD
        PORTA = (PORTA & 0xF0) | (1 << data); // Enable one SSD at a time (digit select)

        // Small delay so the digit remains visible
        for (int delay = 1000; delay--;);
    }
}