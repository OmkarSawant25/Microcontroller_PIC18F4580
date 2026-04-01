/*
 * File:   mian.c
 * Author: OMKAR SAWANT
 *
 * Created on 31 March, 2026, 10:17 AM
 */


#include <xc.h>

// SSD Digit Codes 
#define ZERO 0xE7
#define ONE 0x21
#define TWO 0xCB
#define THREE 0x6B
#define FOUR 0x2D
#define FIVE 0x6E
#define SIX 0xEE
#define SEVEN 0x23
#define EIGHT 0xEF
#define NINE 0x6F

void init_adc();
void init_timer();
void init_ssd();
void display(char *ssd);
void __interrupt() ISR();

unsigned short read_adc(unsigned char channel);

int count = 9999;
int timer_count = 0;
int speed = 10000;


void main(void) {
    unsigned short adc_val;

    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    char ssd[4];

    init_adc();
    init_ssd();
    init_timer();

    while (1) {
        adc_val = read_adc(4);        
        speed = 2000 + ((1023 - adc_val) * 18000) / 1023;
        
//        if(adc_val < 102)
//        {
//            speed = 2000;
//        }
//        else if(adc_val < 204)
//        {
//            speed = 4000;
//        }
//        else if(adc_val < 306)
//        {
//            speed = 6000;
//        }
//        else if(adc_val < 408)
//        {
//            speed = 8000;
//        }
//        else if(adc_val < 510)
//        {
//            speed = 10000;
//        }
//        else if(adc_val < 612)
//        {
//            speed = 12000;
//        }
//        else if(adc_val < 714)
//        {
//            speed = 14000;
//        }
//        else if(adc_val < 816)
//        {
//            speed = 16000;
//        }
//        else if(adc_val < 918)
//        {
//            speed = 18000;
//        }else
//        {
//            speed = 20000;
//        }
        
        ssd[0] = digit[(count / 1000)];
        ssd[1] = digit[(count / 100) % 10];
        ssd[2] = digit[(count / 10) % 10];
        ssd[3] = digit[count % 10];
        
        display(ssd);
    }
    return;
}

void init_adc() {
    ADFM = 1;

    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;

    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;

    GODONE = 0;

    VCFG0 = 0;
    VCFG1 = 1;

    ADRESH = 0;
    ADRESL = 0;

    ADON = 1;
}

void init_ssd() {
    TRISD = 0X00;
    TRISA = TRISA & 0XF0;
    PORTD = 0X00;
}

void init_timer()
{
    GIE = 1;
    PEIE = 1;
    TMR2IE = 1;
    TMR2IF = 0;
    
    T2CKPS0 = 0;
    T2CKPS1 = 0;
    PR2 = 249;
    TMR2 = 0;
    TMR2ON = 1;      
}

unsigned short read_adc(unsigned char channel) {
    unsigned short reg_val;
    ADCON0 = (ADCON0 & 0XC3) | (channel << 2);
    GO = 1;
    while (GO);
    reg_val = (ADRESH << 8) | ADRESL;
    return reg_val;
}

void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data];
        PORTA = (PORTA & 0XF0) | (1 << data);
        for (int delay = 1000; delay--;);
    }
}

void __interrupt() ISR()
{
    if(TMR2IF)
    {
        TMR2IF = 0;
        timer_count++;
        if(timer_count >= speed)
        {
            timer_count = 0;
            if(count > 0)
                count--;
        }
    }
}
