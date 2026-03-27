#include <xc.h>

void __interrupt()_ISR();
void init_timer0();
void init_timer1();
void init_timer2();
void init_port();

static unsigned int count_0 = 0;
static unsigned int count_1 = 0;
static unsigned int count_2 = 0;

void main(void) {
    init_port();

    init_timer0();
    init_timer1();
    init_timer2();

    while (1);

    return;
}

void __interrupt()_ISR() {
    if (INTCONbits.TMR0IF) {
        TMR0 = TMR0 + 8;
        INTCONbits.TMR0IF = 0;

        if (count_0++ == 10000) {
            count_0 = 0;
            PORTBbits.RB0 = !PORTBbits.RB0;
        }
    }
    if (TMR1IF) {
        TMR1 = TMR1 + 3038;
        TMR1IF = 0;

        if (count_1++ == 40) {
            count_1 = 0;
            PORTBbits.RB1 = !PORTBbits.RB1;
        }
    }
    if (TMR2IF) {
        TMR2IF = 0;

        if (count_2++ == 10000) {
            count_2 = 0;
            PORTBbits.RB2 = !PORTBbits.RB2;
        }
    }
}

void init_timer0() {
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    T0CONbits.T08BIT = 1; // 8-bit
    T0CONbits.T0CS = 0; // internal clock
    T0CONbits.PSA = 1; // no prescaler

    TMR0 = 0;

    T0CONbits.TMR0ON = 1;
}

void init_timer1() {
    TRISBbits.TRISB1 = 0;
    PORTBbits.RB1 = 0;

    TMR1IF = 0;
    TMR1IE = 1;

    /* Timer1 configuration */
    TMR1CS = 0; // internal clock
    T1CKPS0 = 0; // prescaler 1:1
    T1CKPS1 = 0;

    TMR1 = 3036; // direct preload

    TMR1ON = 1;
}

void init_timer2() {
    TRISBbits.TRISB2 = 0;
    PORTBbits.RB2 = 0;

    TMR2IF = 0;
    TMR2IE = 1;

    /* Timer2 configuration */
    T2CKPS0 = 0; // prescaler 1:1
    T2CKPS1 = 0;

    PR2 = 249;
    TMR2 = 0;

    TMR2ON = 1;
}

void init_port() {
    GIE = 1;
    PEIE = 1;
}