#include <xc.h>

void __interrupt() isr(void);

unsigned int count = 0;

void main()
{
    TRISBbits.TRISB2 = 0;
    PORTBbits.RB2 = 0;

    /* Interrupt enable */
    GIE = 1;
    PEIE = 1;

    TMR2IF = 0;
    TMR2IE = 1;

    /* Timer2 configuration */
    T2CKPS0 = 0;   // prescaler 1:1
    T2CKPS1 = 0;

    PR2 = 249;
    TMR2 = 0;

    TMR2ON = 1;

    while(1);
}

void __interrupt() isr()
{
    if(TMR2IF)
    {
        TMR2IF = 0;

        count++;

        if(count == 20000)     // 1 second
        {
            RB2 = !RB2;
            count = 0;
        }
    }
}