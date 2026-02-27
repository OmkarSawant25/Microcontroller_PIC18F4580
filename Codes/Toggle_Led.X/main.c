/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 25 February, 2026, 4:06 PM
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
