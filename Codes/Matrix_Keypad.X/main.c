/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 18 March, 2026, 4:44 PM
 */


#include <xc.h>

#define ROW1 PORTBbits.RB5
#define ROW2 PORTBbits.RB6
#define ROW3 PORTBbits.RB7

#define COL1 PORTBbits.RB1
#define COL2 PORTBbits.RB2
#define COL3 PORTBbits.RB3
#define COL4 PORTBbits.RB4

#define LEVEL 0
#define EDGE 1

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

char once = 1;

char scan_switch() {

    ROW1 = 0;
    ROW2 = 1;
    ROW3 = 1;

    if (COL1 == 0) {
        return ONE;
    } else if (COL2 == 0) {
        return FOUR;
    } else if (COL3 == 0) {
        return SEVEN;
    } else if (COL4 == 0) {
        return 10;
    }

    ROW1 = 1;
    ROW2 = 0;
    ROW3 = 1;

    if (COL1 == 0) {
        return TWO;
    } else if (COL2 == 0) {
        return FIVE;
    } else if (COL3 == 0) {
        return EIGHT;
    } else if (COL4 == 0) {
        return 11;
    }

    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 0;
    ROW3 = 0; // Because of instruction cycle


    if (COL1 == 0) {
        return THREE;
    } else if (COL2 == 0) {
        return SIX;
    } else if (COL3 == 0) {
        return NINE;
    } else if (COL4 == 0) {
        return 12;
    }
    return 0x0F;
}

void display(char value) {

    PORTD = value;
    PORTA = (PORTA & 0xF0) | (1); // Enable one digit

}

char triggering_type(char detect_type, char key) {
    if (detect_type == LEVEL) {
        return key;
    } else if (detect_type == EDGE) {
        if ((key != 0x0F) && once) {
            once = 0;
            return key;
        } else if (key == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}

void main(void) {
    TRISB = 0X1E; //RB1 - RB3 as OUTPUT, RB4 - RB7 as INPUT
    INTCON2bits.RBPU = 0; // Enable PORTB pull up pin

    PORTB = PORTB | 0XE0; // To ensure that our input bits are 1 we do this

    TRISD = 0x00; // PORTD as output (segment data)
    TRISA &= 0xF0; // Lower 4 bits of PORTA as output (digit select)

    PORTA &= 0xF0; // Initially disable all digits

    char key;
    char value;

    while (1) {
        key = scan_switch();
        value = triggering_type(EDGE, key);
        display(key);
    }
    return;
}
