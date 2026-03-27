/*
 * File: main.c
 * Author: Omkar Ashok Sawant
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

// Function declaration
char read_switch(char detect_type);

void main(void) {
    TRISB = 0x00; // Configure PORTB as output (LEDs)
    TRISC |= 0x0F; // Configure lower 4 bits of PORTC as input (switches)

    PORTB = 0X00; // Initially turn OFF all LEDs

    unsigned long int delay = 0;

    while (1) {
        key = read_switch(LEVEL); // Read switch input

        // Delay for stable LED display
        if (delay++ == 100000) {
            delay = 0;

            if (key == SW1) {
                // Turn ON one LED
                PORTB = 0X01;
            }
            else if (key == SW2) {
                // Turn ON two LEDs
                PORTB = 0X03;
            }
            else if (key == SW3) {
                // Alternate LEDs ON
                PORTB = 0XAA;
            }
            else if (key == SW4) {
                // Turn ON all LEDs
                PORTB = 0XFF;
            }
            else {
                // No switch pressed ? all LEDs OFF
                PORTB = 0X00;
            }
        }
    }
    return;
}

// Function to read switch from digital keypad

char read_switch(char detect_type) {
    if (detect_type == LEVEL)
        return PORTC & 0x0F;

    else if (detect_type == EDGE) {
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0;
            return PORTC & 0x0F;
        }
        else if ((PORTC & 0x0F) == 0x0F)
            once = 1;
    }

    return 0x0F;
}