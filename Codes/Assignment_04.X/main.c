/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * Program: Implement a 4 digit key press counter
 * Short press  -> increment counter
 * Long press   -> reset counter to 0
 */

#include <xc.h>

/* Seven Segment Display Hex Codes */
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

#define LEVEL 0      // Detect key while it is pressed
#define EDGE 1       // Detect only when key is pressed once

// Switch connected to keypad
#define SW1 0x0E

char read_digit_keypad(char detect_type);
void display(char *ssd);

/* Global Variables */
char once = 1; // Used for edge detection
int count = 0; // Counter value
char key; // Stores pressed key
int time = 0; // Used to detect long press

void main(void) {
    TRISD = 0X00; // PORTD as output (SSD segment control)
    TRISA = TRISA & 0xF0; // Lower 4 bits of PORTA as output (digit select)
    TRISC = TRISC | 0x0F; // Lower 4 bits of PORTC as input (keypad)

    PORTD = 0X00; // Clear PORTD
    PORTA &= 0xF0; // Clear digit select lines

    // Array containing SSD patterns for digits 0?9 
    char digits[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    char ssd[4]; // Array to store 4 digits for display

    while (1) {
        // Read keypad using level detection
        key = read_digit_keypad(LEVEL);

        if (key == SW1) // If switch is pressed
        {
            time++; // Increase press duration counter
            if (time >= 150) // If pressed for long time
            {
                count = 0; // Reset counter
            }
        } else if (time < 150 && time > 0) // Short press detected
        {
            count++; // Increment counter
            time = 0;
        } else {
            time = 0;
        }
        // Convert counter value into individual digits for SSD 
        ssd[0] = digits[count / 1000]; // Thousands digit
        ssd[1] = digits[(count / 100) % 10]; // Hundreds digit
        ssd[2] = digits[(count / 10) % 10]; // Tens digit
        ssd[3] = digits[count % 10]; // Units digit

        // Display the digits 
        display(ssd);
    }

    return;
}

char read_digit_keypad(char detect_type) {
    if (detect_type == LEVEL) {
        return PORTC & 0X0F; // Return lower 4 bits of PORTC
    } else if (detect_type == EDGE) {
        if ((PORTC & 0X0F) != 0XF0 && once) {
            once = 0; // Prevent repeated detection
            return PORTC & 0X0F;
        } else if ((PORTC & 0X0F) == 0X0F) {
            once = 1; // Reset detection when key released
        }
    }
    return 0x0F; // No key pressed
}

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data]; // Send segment pattern
        PORTA = (PORTA & 0XF0) | (1 << data); // Enable corresponding SSD one at a time

        for (int i = 0; i < 1000; i++); // Small delay for visibility
    }
}