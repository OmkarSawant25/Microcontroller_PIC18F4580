/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * PIC18F4580 LED Train Pattern
 */

#include <xc.h>

// Blocking Led Train Pattern
//void main(void) {
//
//    TRISB = 0x00;
//    PORTB = 0X00;
//    int i = 0;
//    while (1) {
//        if (i < 8) 
//        {
//            PORTB = PORTB | (1 << i);
//        } 
//        else if (i >= 8 && i < 16) 
//        {
//            PORTB = PORTB << 1;
//        } 
//        else if (i >= 16 && i < 24) 
//        {
//            PORTB = PORTB | (0x80) >> (i - 16);
//        } 
//        else if (i >= 24 && i < 32) 
//        {
//            PORTB = PORTB >> 1;
//        } 
//        else 
//        {
//            i = -1;
//        }
//
//        i++;
//        for (int delay = 50000; delay--;);
//    }
//    return;
//}


// Non Blocking Led Train Pattern
void main(void) {

    TRISB = 0x00;
    PORTB = 0X00;
    unsigned int delay = 0;
    unsigned int i = 0;

    while (1) {
        if (delay++ == 50000) {
            delay = 0;
            if (i < 8) {
                PORTB |= (1 << i);
            } else if (i >= 8 && i < 16) {
                PORTB = PORTB << 1;
            } else if (i >= 16 && i < 24) {
                PORTB = PORTB | (0x80) >> (i - 16);
            } else if (i >= 24 && i < 32) {
                PORTB = PORTB >> 1;
            } else {
                i = -1;
            }
            i++;
        }
    }
    return;
}