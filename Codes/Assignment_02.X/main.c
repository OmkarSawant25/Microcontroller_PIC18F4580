/*
 * File: main.c
 * Author: Omkar Ashok Sawant
 * Program: Multiple LED Patterns using Switches
 */

#include <xc.h>

// Switch values from digital keypad
#define SW1 0x0E
#define SW2 0x0D
#define SW3 0x0B
#define SW4 0x07

#define LEVEL 0
#define EDGE 1

char key;
char once = 1;
int i = 0;

// Function declarations
char read_switch(char detect_type);
void pattern_1();
void pattern_2();

void main(void) {
    TRISB = 0x00; // Configure PORTB as output (LEDs)
    TRISC |= 0x0F; // Configure lower 4 bits of PORTC as input (switches)

    PORTB = 0x00; // Initially turn OFF all LEDs

    unsigned long int delay = 0;
    int flag = 0; // Variable to select pattern

    while (1) {
        key = read_switch(LEVEL); // Read switch input

        // If switch 1 is pressed -> Pattern 1
        if (key == SW1) {
            PORTB = 0x00;
            i = 0;
            flag = 1;
        }

        // If switch 2 is pressed -> Pattern 2
        if (key == SW2) {
            PORTB = 0x00;
            i = 0;
            flag = 2;
        }

        // If switch 3 is pressed -> Alternate LED blinking
        if (key == SW3) {
            PORTB = 0XAA;
            flag = 3;
        }

        // If switch 4 is pressed -> Nibble blinking
        if (key == SW4) {
            PORTB = 0XF0;
            flag = 4;
        }

        // Delay for visible LED pattern
        if (delay++ == 25000) {
            delay = 0;

            if (flag == 1) {
                pattern_1(); // Run Pattern 1
            } else if (flag == 2) {
                pattern_2(); // Run Pattern 2
            } else if (flag == 3 || flag == 4) {
                PORTB = ~PORTB; // Blink LEDs
            }
        }
    }
    return;
}

// Function to read switch input

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

// Pattern 1: Train pattern Left<->Right

void pattern_1() {
    if (i < 8) {
        PORTB = PORTB | (1 << i);
    } else if (i < 16) {
        PORTB = PORTB << 1;
    } else if (i < 24) {
        PORTB = PORTB | (0X80 >> (i - 16));
    } else if (i < 32) {
        PORTB >>= 1;
    } else {
        i = -1;
    }
    i++;
}

// Pattern 2: LEDs ON Left<->Right and OFF Left<->Right

void pattern_2() {
    if (i < 8) {
        PORTB = PORTB | (1 << i);
    } else if (i < 16) {
        PORTB = PORTB << 1;
    } else {
        i = -1;
    }
    i++;
}