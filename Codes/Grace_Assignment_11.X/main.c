/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 31 March, 2026, 12:39 PM
 */

#include <xc.h>
#include "clcd.h"

#define _XTAL_FREQ 8000000

#define ROW1 PORTBbits.RB5
#define ROW2 PORTBbits.RB6
#define ROW3 PORTBbits.RB7

#define COL1 PORTBbits.RB0
#define COL2 PORTBbits.RB1
#define COL3 PORTBbits.RB2

#define LEVEL 0
#define EDGE 1

char once = 1;

static void init_config(void) {
    init_clcd();
}

char triggering_type(char type, char key);
char scan_switch();
void init_matrix();

void main(void) {
    init_config();
    init_matrix();
    char str[] = "9999999999";
    char msg[] = "DOWN COUNTER";
    char key;
    char value;

    while (1) {
        key = scan_switch();
        value = triggering_type(EDGE, key);
        
        str[9]--;

        if (str[9] == '0') {
            int i = 8;
            for (; i >= 0; i++) {
                if (str[i] != '0') {
                    str[i]--;
                    break;
                }
            }
            i++;
            for (; i < 10; i++) {
                str[i] = '9';
            }
        }
        clcd_print(msg, LINE1(0));
        clcd_print(str, LINE2(0));
        __delay_ms(500);
    }

    return;
}

char triggering_type(char type, char key)
{
    if (type == LEVEL) {
        return key;
    } else if (type == EDGE) {
        if ((PORTC & 0X0F) != 0x0F && once) {
            once = 0;
            return key;
        } else if ((PORTC & 0x0F) == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}

char scan_switch() {
    ROW1 = 0;
    ROW2 = 1;
    ROW3 = 1;

    if (COL1 == 0) {
        return '1';
    }

    ROW1 = 1;
    ROW2 = 0;
    ROW3 = 1;

    if (COL1 == 0) {
        return '2';
    }

    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 0;

    if (COL1 == 0) {
        return '3';
    }

}

void init_matrix()
{
    TRISB = 0X1E;
    RBPU = 1;
    PORTB = PORTB | 0XE0;
}