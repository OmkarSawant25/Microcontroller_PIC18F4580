/*
 * File:   Assign10.c
 * Author: HP
 *
 * Created on 23 March, 2026, 4:24 PM
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>

#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7

#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0
#define _XTAL_FREQ                  20000000
#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME							clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE   0x33

#define ROW1 PORTBbits.RB5
#define ROW2 PORTBbits.RB6
#define ROW3 PORTBbits.RB7

#define COL1 PORTBbits.RB1
#define COL2 PORTBbits.RB2
#define COL3 PORTBbits.RB3
#define COL4 PORTBbits.RB4

#define LEVEL 0
#define EDGE 1

unsigned int delay = 0;
unsigned int led_delay = 0;

void clcd_write(unsigned char byte, unsigned char control_bit) {
    CLCD_RS = control_bit;
    CLCD_PORT = byte;

    /* Should be atleast 200ns */
    CLCD_EN = HI;
    CLCD_EN = LO;

    PORT_DIR = INPUT;
    CLCD_RW = HI;
    CLCD_RS = INSTRUCTION_COMMAND;

    do {
        CLCD_EN = HI;
        CLCD_EN = LO;
    } while (CLCD_BUSY);

    CLCD_RW = LO;
    PORT_DIR = OUTPUT;
}

void init_clcd() {
    /* Set PortD as output port for CLCD data */
    TRISD = 0x00;
    /* Set PortC as output port for CLCD control */
    TRISC = TRISC & 0xF8;

    CLCD_RW = LO;


    /* Startup Time for the CLCD controller */
    __delay_ms(30);

    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(1);

    CURSOR_HOME;
    __delay_us(100);
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr) {
    clcd_write(addr, INSTRUCTION_COMMAND);
    while (*data != '\0') {
        clcd_write(*data++, DATA_COMMAND);
    }
}

void clcd_putch(const unsigned char data, unsigned char addr) {
    clcd_write(addr, INSTRUCTION_COMMAND);
    clcd_write(data, DATA_COMMAND);
}

void blink_cursor(char *str, int index) {

    clcd_print(str, LINE1(0));

    if (delay++ < 2500) {
        clcd_putch('_', LINE2(index));
    } else if (delay < 5000) {
        clcd_putch(' ', LINE2(index));
    } else
        delay = 0;

    for (int i = 0; i < index; i++) {
        clcd_putch('*', LINE2(i));
    }
}

char once = 1;

char scan_switch() {
    ROW1 = 0;
    ROW2 = 1;
    ROW3 = 1;
    if (COL1 == 0)
        return '0';
    else if (COL2 == 0)
        return '1';

    return 0x0F;
}

char switch_trigger(char detect, char key) {
    if (detect == LEVEL) {
        return key;
    } else if (detect == EDGE) {
        if (key != 0x0F && once) {
            once = 0;
            return key;
        } else if (key == 0x0F) {
            once = 1;
        }
    }
    return 0x0F;
}

void display() {
    CLEAR_DISP_SCREEN;
    char str[16] = "Password Correct";
    clcd_print(str, LINE1(0));
    __delay_ms(10000);
    CLEAR_DISP_SCREEN;
}

void failure(char attempt) {
    char str[16] = "Wrong Password";
    char str2[16] = "Attempt left";

    CLEAR_DISP_SCREEN;
    clcd_print(str, LINE1(0));
    clcd_putch(attempt, LINE2(0));
    clcd_print(str2, LINE2(2));
    __delay_ms(2000);
    CLEAR_DISP_SCREEN;
}

void LED_blink(char attempt) {
    if (attempt == '5') {
        RB0 = 0;
    } else if (attempt == '4') {
        if (led_delay++ <= 2000) {
            RB0 = !RB0;
        } else if (led_delay <= 4000) {
            RB0 = !RB0;
        } else {
            led_delay = 0;
        }
    } else if (attempt == '3') {
        if (led_delay++ <= 1500) {
            RB0 = !RB0;
        } else if (led_delay <= 3000) {
            RB0 = !RB0;
        } else {
            led_delay = 0;
        }
    } else if (attempt == '2') {
        if (led_delay++ <= 1000) {
            RB0 = !RB0;
        } else if (led_delay <= 2000) {
            RB0 = !RB0;
        } else {
            led_delay = 0;
        }
    } else if (attempt == '1') {
        if (led_delay++ <= 500) {
            RB0 = !RB0;
        } else if (led_delay <= 1000) {
            RB0 = !RB0;
        } else {
            led_delay = 0;
        }
    } else if (attempt == '0') {
        RB0 = 1;
    }
}

void failure_at_last() {
    char str[16] = "Wrong Password";
    char str2[16] = "Reset the Board";

    CLEAR_DISP_SCREEN;
    clcd_print(str, LINE1(0));
    clcd_print(str2, LINE2(0));
    __delay_ms(2000);
    CLEAR_DISP_SCREEN;
}

void main(void) {

    init_clcd();

    TRISB = 0x1E;
    INTCON2bits.RBPU = 0;

    PORTB = PORTB | 0xE0;
    PORTBbits.RB0 = 0;

    char msg[16] = "Enter Password";
    char org_pass[9] = "11110000";
    char ent_pass[9] = {0};
    int index = 0;

    char key;
    char value;

    char attempt = '5';
    while (1) {

        if (index < 8) {
            blink_cursor(msg, index);

            key = scan_switch();

            value = switch_trigger(EDGE, key);

            if (value == '0') {
                ent_pass[index++] = value;
            } else if (value == '1') {
                ent_pass[index++] = value;
            }

        } else {
            if (attempt == '0') {
                failure_at_last();
            } else {
                ent_pass[index] = '\0';
                if (strcmp(org_pass, ent_pass) == 0) {
                    display();
                    attempt = '5';

                } else {
                    attempt--;
                    failure(attempt);
                }
                if (attempt != '0')
                    index = 0;
            }
        }
        LED_blink(attempt);
    }
    return;
}