#include <xc.h>

void __interrupt() isr(void);

unsigned int count = 0;

void main()
{
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    INTCONbits.GIE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    T0CONbits.T08BIT = 1;   // 8-bit
    T0CONbits.T0CS = 0;     // internal clock
    T0CONbits.PSA = 1;      // no prescaler

    TMR0 = 0;

    T0CONbits.TMR0ON = 1;

    while(1);
}

void __interrupt() isr()
{
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        count++;

        if(count >= 20000)  
        {
            PORTBbits.RB0 ^= 1;
            count = 0;
        }
    }
}