/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * PIC18F4580 LED Train Pattern
 */

#include <xc.h>

// Simple delay
void delay(void)
{
    for (unsigned long i = 0; i < 50000; i++)
        ;
}

void main(void)
{
    TRISB = 0x00; // PORTB as output
    PORTB = 0x00; // All LEDs OFF

    unsigned char i;

    while (1)
    {
        // EXIT: Left to Right (ON one by one)
        for (i = 0; i < 8; i++)
        {
            PORTB |= (1 << i);
            delay();
        }

        // ENTRY: Left to Right (OFF one by one)
        for (i = 0; i < 8; i++)
        {
            PORTB &= ~(1 << i);
            delay();
        }

        // EXIT: Right to Left (ON one by one)
        for (i = 0; i < 8; i++)
        {
            PORTB |= (0x80 >> i);
            delay();
        }

        // ENTRY: Right to Left (OFF one by one)
        for (i = 0; i < 8; i++)
        {
            PORTB &= ~(0x80 >> i);
            delay();
        }
    }
}