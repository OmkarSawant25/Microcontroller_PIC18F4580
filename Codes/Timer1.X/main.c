#include <xc.h>

void __interrupt() isr(void);

unsigned char count = 0;

void main()
{
    TRISBbits.TRISB1 = 0;
    PORTBbits.RB1 = 0;

    /* Interrupt enable */
    GIE = 1;
    PEIE = 1;

    TMR1IF = 0;
    TMR1IE = 1;

    /* Timer1 configuration */
    TMR1CS = 0;   // internal clock
    T1CKPS0 = 0;  // prescaler 1:1
    T1CKPS1 = 0;

    TMR1 = 3036;  // direct preload

    TMR1ON = 1;

    while(1);
}

void __interrupt() isr()
{
    if(TMR1IF)
    {
        TMR1IF = 0;

        TMR1 = 3036;   // reload

        count++;

        if(count == 80)     // 1 second
        {
            RB1 = !RB1;
            count = 0;
        }
    }
}