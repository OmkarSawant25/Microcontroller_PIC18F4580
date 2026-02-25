/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 20 February, 2026, 3:49 PM
 */


#include <xc.h>

void main(void) {
    TRISB = 0x00; // Make the port as output port

    while (1) {
        PORTB = 0xFF; // Turn ON
        for (unsigned int delay = 50000; delay--;);   // Blocking Delay
        PORTB = 0x00; // Turn OFF
        for (unsigned int delay = 50000; delay--;);
    }
    return;
}
