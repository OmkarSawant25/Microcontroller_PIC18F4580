/*
 * File:   main.c
 * Author: Omkar Sawant
 * Title : 4 Digit Key Press Counter with EEPROM Persistence
 *
 * Functionality:
 *  - Short press on SW1 -> Increment counter
 *  - Long press on SW1  -> Reset counter to 0000
 *  - Press SW2          -> Store current count in internal EEPROM
 *  - On power ON / reset -> Counter restores previous stored value
 */

#include <xc.h>

/* SSD Segment Codes for digits 0-9 */
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

/* Key Detection Modes */
#define LEVEL 0      // Detect key continuously while pressed
#define EDGE  1      // Detect only once per press

/* Keypad Switch Values */
#define SW1 0x0E     // Count / Reset switch
#define SW2 0x0D     // Store switch

/* Function Prototypes */
char read_digit_keypad(char detect_type);
void write_internal_eeprom(unsigned char address, unsigned char data);
unsigned char read_internal_eeprom(unsigned char address);
void display(char *ssd);

/* Global Variables */
char once = 1; // Used for EDGE detection logic
int count = 0; // Stores current counter value
char key; // Stores key pressed
int time = 0; // Used to measure long press duration

void main(void) {
    /* Configure Ports */
    TRISD = 0x00; // PORTD : SSD segment output
    TRISA &= 0xF0; // Lower nibble : SSD digit enable lines
    TRISC |= 0x0F; // Lower nibble : Keypad input

    PORTD = 0x00;
    PORTA &= 0xF0;

    /* Restore previous stored count from EEPROM */
    count = read_internal_eeprom(0x00);
    count = count * 10 + read_internal_eeprom(0x01);
    count = count * 10 + read_internal_eeprom(0x02);
    count = count * 10 + read_internal_eeprom(0x03);

    /* SSD digit lookup table */
    char digits[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

    char ssd[4]; // Stores SSD patterns for 4 digits

    while (1) {
        /* Read keypad using LEVEL detection */
        key = read_digit_keypad(LEVEL);

        /* STORE operation */
        if (key == SW2) {
            /* Store each digit separately in EEPROM */
            write_internal_eeprom(0x00, count / 1000);
            write_internal_eeprom(0x01, (count / 100) % 10);
            write_internal_eeprom(0x02, (count / 10) % 10);
            write_internal_eeprom(0x03, count % 10);
        }
        /* COUNT / RESET operation */
        else if (key == SW1) {
            time++; // Measure how long key is pressed

            /* Long press detected */
            if (time >= 150) {
                count = 0;
            }
        }
        /* Short press detected (key released before long threshold) */
        else if (time < 150 && time > 0) {
            count++;

            /* Maintain 4 digit range */
            if (count > 9999)
                count = 0;

            time = 0;
        } else {
            time = 0;
        }

        /* Convert count into SSD patterns */
        ssd[0] = digits[count / 1000];
        ssd[1] = digits[(count / 100) % 10];
        ssd[2] = digits[(count / 10) % 10];
        ssd[3] = digits[count % 10];

        /* Refresh display continuously */
        display(ssd);
    }
}

/* Keypad Reading Function */
char read_digit_keypad(char detect_type) {
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

    return 0x0F; // No key pressed
}

/* SSD Multiplex Display Function */
void display(char *ssd) {
    for (int digit = 0; digit < 4; digit++) {
        PORTD = ssd[digit]; // Send segment data
        PORTA = (PORTA & 0xF0) | (1 << digit); // Enable corresponding digit

        for (int delay = 0; delay < 1000; delay++); // Small persistence delay
    }
}

/* EEPROM Write Function */
void write_internal_eeprom(unsigned char address, unsigned char data) {
    EEADR = address;
    EEDATA = data;

    EECON1bits.EEPGD = 0; // Point to data EEPROM
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;

    GIE = 0; // Disable interrupts

    EECON2 = 0x55; // Mandatory unlock sequence
    EECON2 = 0xAA;

    EECON1bits.WR = 1; // Start write

    GIE = 1; // Enable interrupts

    while (!PIR2bits.EEIF); // Wait till write completes
    PIR2bits.EEIF = 0;
}

/* EEPROM Read Function */
unsigned char read_internal_eeprom(unsigned char address) {
    EEADR = address;

    EECON1bits.WREN = 0; // Disable write
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;

    EECON1bits.RD = 1; // Start read

    return EEDATA;
}