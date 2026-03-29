/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 27 March, 2026
 *
 * Title: LED Dimmer using PWM (Timer ISR Based)
 *
 * Description:
 * This program implements a PWM-based LED dimmer using Timer2 interrupt.
 * The brightness of the LED connected to RB0 is controlled using a 
 * potentiometer connected to ADC channel AN4.
 *
 * Working:
 * - Potentiometer provides analog voltage
 * - ADC converts it into digital value (0?1023)
 * - Duty cycle is calculated from ADC value
 * - Timer2 ISR generates PWM signal
 * - LED brightness varies according to duty cycle
 */

#include <xc.h>

// Function declarations
void __interrupt() ISR();
void init_adc();
void init_timer2();
unsigned short read_adc(unsigned char channel);

// Global variables
unsigned int duty_cycle = 0; // PWM duty cycle (0?100)
int period = 100; // PWM period
int timer_cycle = 0; // Timer cycle counter

void main(void) {

    TRISB = 0X00; // Set PORTB as output
    RB0 = 1; // Initialize LED ON

    init_adc(); // Initialize ADC module
    init_timer2(); // Initialize Timer2 for PWM

    unsigned short int value; // ADC value

    while (1) {

        value = read_adc(4); // Read potentiometer (AN4)

        // Convert ADC value (0?1023) to duty cycle (approx 0?100)
        duty_cycle = (value * 100) / 1023;
//        duty_cycle = value / 10.23;
    }

    return;
}

// Interrupt Service Routine for Timer2
void __interrupt() ISR() {

    if (TMR2IF) { // Check Timer2 interrupt flag
        TMR2IF = 0; // Clear interrupt flag
        timer_cycle++; // Increment cycle count
        // PWM generation
        if (timer_cycle < duty_cycle) {
            RB0 = 1; // LED ON (HIGH time)
        }
        else if (timer_cycle < period) {
            RB0 = 0; // LED OFF (LOW time)
        }
        else {
            timer_cycle = 0; // Reset cycle
        }
    }
}

// ADC Initialization

void init_adc() {

    ADFM = 1; // Right justified result

    // Acquisition time
    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;

    // Conversion clock
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;

    GODONE = 0; // Ensure no conversion in progress

    // Voltage reference = VDD and VSS
    VCFG1 = 0;
    VCFG0 = 0;

    // Clear ADC registers
    ADRESH = 0;
    ADRESL = 0;

    ADON = 1; // Enable ADC
}

// ADC Read Function

unsigned short read_adc(unsigned char channel) {

    unsigned short reg_value;

    // Select ADC channel
    ADCON0 = (ADCON0 & 0XC3) | (channel << 2);

    GO = 1; // Start conversion

    while (GO); // Wait until conversion completes

    // Combine result (10-bit)
    reg_value = (ADRESH << 8) | ADRESL;

    return reg_value;
}

// Timer2 Initialization for PWM

void init_timer2() {

    /* Enable interrupts */
    GIE = 1; // Global interrupt enable
    PEIE = 1; // Peripheral interrupt enable

    TMR2IF = 0; // Clear Timer2 interrupt flag
    TMR2IE = 1; // Enable Timer2 interrupt

    /* Timer2 configuration */
    T2CKPS0 = 0; // Prescaler 1:1
    T2CKPS1 = 0;

    PR2 = 249; // Period register
    TMR2 = 0; // Initialize timer

    TMR2ON = 1; // Start Timer2
}