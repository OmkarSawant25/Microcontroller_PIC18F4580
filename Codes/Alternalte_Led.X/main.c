/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 26 February, 2026, 10:24 AM
 */


#include <xc.h>

void main(void) {
    unsigned int delay = 0;
    
    TRISB = 0x00;
    PORTB = 0xAA;
    
    while(1)
    {
        if(delay++ == 50000)
        {
            delay = 0;
            PORTB = ~PORTB;
        }
    }
    return;
}

