/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * Program: Right scrolling message marquee on CLCD
 * Message scrolls right continuously with ~0.5 Hz speed
 */

#include <xc.h>
#include "clcd.h"
#define XTAL 8000000

// Initialize CLCD

static void init_config(void) {
    init_clcd();
}

void main(void) {
    init_config();

    char str[17] = "Hello World     "; // message with spaces for smooth scroll

    while (1) {
        clcd_print(str, LINE1(0)); // display string

        __delay_ms(2000); // delay for ~0.5 Hz

        // Right scroll logic
        char last = str[15]; // store last character

        for (int i = 15; i > 0; i--) {
            str[i] = str[i - 1]; // shift right
        }

        str[0] = last; // move last to first
    }
}

//void main(void) {
//    init_config();
//    char str[20] = "Hello World     ";
//
//    int count = 0;
//    while (1) {
//        clcd_print(str, LINE1(0));
//
//        if (count++ == 1000) {
//            count = 0;
//            char last = str[15];
//            for (int i = 15; i > 0; i--) {
//                str[i] = str[i - 1];
//            }
//            str[0] = last;
//        }
//    }
//}