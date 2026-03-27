/*
 * File: main.c
 * Author: Omkar Ashok Sawant
 * Program: LED Train Pattern with Direction Control
 */

#include <xc.h>

#define SW1    0x0E
#define LEVEL  0
#define EDGE   1

char once = 1;
char key;
int i = 0;

// Function declarations
char read_switch(char detect_type);
void pattern_1();
void pattern_2();

void main(void) {
    TRISB = 0x00; // Configure PORTB as output (LEDs)
    TRISC |= 0x0F; // Configure lower 4 bits of PORTC as input (switch)

    PORTB = 0x00; // Initially turn OFF all LEDs

    unsigned long int delay = 0;
    int flag = 1; // Direction control flag

    while (1) {
        key = read_switch(EDGE); // Read switch using edge detection

        // If SW1 is pressed change the direction
        if (key == SW1) {
            // Adjust position so pattern continues smoothly
            if (i != 0) {
                i = 15 - i;
            }
            flag = !flag; // Toggle direction
        }

        // Non Blocking delay for LED speed
        if (delay++ == 50000) {
            delay = 0;
            if (flag)
                pattern_1(); // Left -> Right pattern
            else
                pattern_2(); // Right -> Left pattern
        }
    }
}

// Function to read switch

char read_switch(char detect_type) {
    if (detect_type == LEVEL) {
        return PORTC & 0x0F;
    } else if (detect_type == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return PORTC & 0x0F;
        } else if ((PORTC & 0x0F) == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}

// Pattern 1: Train pattern Left -> Right

void pattern_1() {
    // Turn ON LEDs one by one
    if (i < 8) {
        PORTB |= (1 << i);
    }        // Turn OFF LEDs one by one
    else if (i < 16) {
        PORTB <<= 1;
    } else {
        i = -1;
    }
    i++; // Move to next LED
}

// Pattern 2: Train pattern Right -> Left

void pattern_2() {
    // Turn ON LEDs from Right side
    if (i < 8) {
        PORTB |= (0x80 >> i);
    }        // Turn OFF LEDs from Right side
    else if (i < 16) {
        PORTB >>= 1;
    } else {
        i = -1;
    }
    i++; // Move to next LED
}