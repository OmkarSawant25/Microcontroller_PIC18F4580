/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 5 March, 2026, 4:22 PM
 */

#include <xc.h>

void main()
{
    TRISB = 0x01; // RB0 input
    RB7 = 0;      // LED OFF

    GIE = 1;    // Enable global interrupt
    PEIE = 1;   // Enable peripheral interrupt
    INT0IE = 1; // Enable external interrupt

    INT0IF = 0; // Clear interrupt flag

    while (1);
    return;
}

void __interrupt() isr()
{
    if(INT0IF)
    {
        INT0IF = 0;     // Clear interrupt flag
        RB7 = !RB7;     // Toggle LED
    }
}


// void main(void) {
//     //    TRISB = 0x01;
//     //    RB7 = 0;

//     TRISB0 = 1;
//     TRISB7 = 0;
//     PORTBbits.RB7 = 0;

//     //    INTCON = 0XD0;
//     GIE = 1;
//     PEIE = 1;
//     INT0IE = 1;
//     INT0IF = 0;
//     while (1);

//     return;
// }

// void __interrupt() _ISR()
// {
//     if (INT0IF)
//     {
//         INT0IF = 0;
//         PORTBbits.RB7 = !PORTBbits.RB7;
//     }
// }