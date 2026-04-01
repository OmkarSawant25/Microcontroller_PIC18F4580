#include <xc.h>

// Row pins (Output)
#define ROW1 PORTBbits.RB5
#define ROW2 PORTBbits.RB6
#define ROW3 PORTBbits.RB7

// Column pins (Input)
#define COL1 PORTBbits.RB1
#define COL2 PORTBbits.RB2
#define COL3 PORTBbits.RB3
#define COL4 PORTBbits.RB4

// Detection modes
#define LEVEL 0
#define EDGE 1

// 7-Segment codes
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

char once = 1;   // Used for edge detection (single press detection)

// Function to scan keypad
char scan_switch() {

    // Activate ROW1 (LOW), others HIGH
    ROW1 = 0;
    ROW2 = 1;
    ROW3 = 1;

    // Check columns for key press
    if (COL1 == 0) {
        return ONE;     // Key '1'
    } else if (COL2 == 0) {
        return FOUR;    // Key '4'
    } else if (COL3 == 0) {
        return SEVEN;   // Key '7'
    } else if (COL4 == 0) {
        return 10;      // Special key (*)
    }

    // Activate ROW2
    ROW1 = 1;
    ROW2 = 0;
    ROW3 = 1;

    if (COL1 == 0) {
        return TWO;     // Key '2'
    } else if (COL2 == 0) {
        return FIVE;    // Key '5'
    } else if (COL3 == 0) {
        return EIGHT;   // Key '8'
    } else if (COL4 == 0) {
        return 11;      // Special key (0)
    }

    // Activate ROW3
    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 0;
    ROW3 = 0; // Small delay (instruction cycle)

    if (COL1 == 0) {
        return THREE;   // Key '3'
    } else if (COL2 == 0) {
        return SIX;     // Key '6'
    } else if (COL3 == 0) {
        return NINE;    // Key '9'
    } else if (COL4 == 0) {
        return 12;      // Special key (#)
    }

    return 0x0F; // No key pressed
}

// Function to display value on 7-segment
void display(char value) {

    PORTD = value; // Send segment data

    // Enable only 1st digit (no multiplexing here)
    PORTA = (PORTA & 0xF0) | (1);
}

// Function for LEVEL or EDGE detection
char triggering_type(char detect_type, char key) {

    if (detect_type == LEVEL) {
        return key; // Continuous detection
    } 
    
    else if (detect_type == EDGE) {
        
        // Detect key only once
        if ((key != 0x0F) && once) {
            once = 0;   // Disable further detection
            return key; // Return key once
        } 
        
        // Reset when key released
        else if (key == 0x0F) {
            once = 1;
        }
    }

    return 0x0F; // No valid key
}

void main(void) {

    TRISB = 0X1E; // RB1-RB4 input (columns), RB5-RB7 output (rows)

    INTCON2bits.RBPU = 0; // Enable internal pull-ups

    PORTB = PORTB | 0XE0; // Set row lines HIGH initially

    TRISD = 0x00; // SSD segments output
    TRISA &= 0xF0; // Digit control output

    PORTA &= 0xF0; // Disable all digits initially

    char key;
    char value;

    while (1) {

        key = scan_switch(); // Scan keypad

        value = triggering_type(EDGE, key); // Apply edge detection

        display(key); // Display pressed key
    }

    return;
}