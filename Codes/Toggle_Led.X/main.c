/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 25 February, 2026, 4:06 PM
 */


#include <xc.h>
#include <pic18f4580.h>

//void main(void) {
//    unsigned int delay = 0;
//
//    TRISB = 0x00;
//    PORTB = 0x01;
//
//    while (1) {
//        if (delay++ == 50000) {
//            delay = 0;
//            //            PORTB = ~PORTB;
//            PORTBbits.RB0 = !PORTBbits.RB0;
//        }
//    }
//    return;
//}

//void main(void) {
//
//    TRISB = 0x00;
//    PORTB = 0x01;
//
//    while (1) {
//        PORTBbits.RB0 = 1;
//        for (int delay = 900; delay--;);
//        PORTBbits.RB0 = 0;
//        for (int delay = 100; delay--;);
//    }
//    return;
//}

void main(void) {

    TRISB = 0x00;
    PORTB = 0x01;

    int delay = 0;
    while (1) {
        delay++;
        if (delay < 100) {
            PORTBbits.RB0 = 1;
        } else if (delay < 1000) {
            PORTBbits.RB0 = 0;
        } else {
            delay = 0;
        }
    }
    return;
}
