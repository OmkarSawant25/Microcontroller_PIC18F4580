/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 27 March, 2026
 *
 * Title: Temperature Sensor Node with SSD Display
 *
 * Description:
 * This program reads temperature from an LM35 sensor using ADC
 * and displays the temperature on a 4-digit Seven Segment Display (SSD)
 * in the format XX.C.
 *
 * Working:
 * - LM35 outputs 10mV per °C
 * - ADC converts analog voltage to digital value (0?1023)
 * - Digital value is converted to millivolts
 * - Temperature is calculated and displayed on SSD
 *
 * Hardware Used:
 * - PIC Microcontroller
 * - LM35 Temperature Sensor
 * - 4-digit Seven Segment Display
 */

#include <xc.h>

// SSD segment codes for digits 0?9
#define ZERO  0xE7
#define ONE   0x21
#define TWO   0xCB
#define THREE 0x6B
#define FOUR  0x2D
#define FIVE  0x6E
#define SIX   0xEE
#define SEVEN 0x23
#define EIGHT 0xEF
#define NINE  0x6F

// SSD code for character 'C'
#define C     0xC6

// SSD code for decimal point (DP segment)
#define DOT   0x10

// Function prototypes
unsigned short read_adc(unsigned char channel);
void display(char *ssd);
void init_adc();

void main(void) {

    init_adc();              // Initialize ADC module

    TRISD = 0X00;           // PORTD as output (SSD segments)
    TRISA = TRISA & 0XF0;   // Lower 4 bits of PORTA as output (digit select)

    PORTA &= 0XF0;          // Clear lower 4 bits

    // Lookup table for digits
    char digit[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

    char ssd[4];            // Array to hold 4-digit display data

    unsigned short int value;   // ADC value (0?1023)
    // unsigned int volt;         
    unsigned int milli_volt;    // Voltage in millivolts
    unsigned int celcius;       // Temperature in °C

    while (1) {
        value = read_adc(6);    // Read ADC from channel AN6
        // Convert ADC value to millivolts
        // Step size = 4.887 mV
        milli_volt = value * 4887 / 1000;
        // LM35: 10 mV = 1 Degree C
        celcius = milli_volt / 10;

        // volt = value * 0.004887;
        // milli_volt = volt * 1000;
        // celcius = milli_volt / 10;

        // Display format: [Tens][Ones][.][C]
        ssd[0] = digit[celcius / 10];   // Tens place
        ssd[1] = digit[celcius % 10];   // Ones place
        ssd[2] = DOT;                    // Decimal point
        ssd[3] = C;                      // Character 'C'

        display(ssd);                    // Send data to SSD
    }

    return;
}

// ADC Initialization Function
void init_adc() {

    ADFM = 1;   // Right justified result (10-bit)

    // Acquisition time selection
    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;

    // ADC conversion clock selection
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;

    GODONE = 0; // Ensure no conversion is running

    // Voltage reference = VDD and VSS
    VCFG1 = 0;
    VCFG0 = 0;

    // Clear ADC result registers
    ADRESH = 0;
    ADRESL = 0;

    ADON = 1;   // Enable ADC module
}

// Function to read ADC value from selected channel
unsigned short read_adc(unsigned char channel) {
    unsigned short reg_value;
    // Select ADC channel
    ADCON0 = (ADCON0 & 0XC3) | (channel << 2);
    GO = 1;             // Start ADC conversion
    while (GO);         // Wait until conversion completes
    // Combine high and low result registers
    reg_value = (ADRESH << 8) | ADRESL;
    return reg_value;
}

// Function to display data on 4-digit SSD (multiplexing)
void display(char *ssd) {
    for (int data = 0; data < 4; data++) {
        PORTD = ssd[data];  // Send segment data to SSD
        // Enable one digit at a time (multiplexing)
        PORTA = (PORTA & 0xF0) | (1 << data);
        // Small delay for persistence of vision
        for (int delay = 1000; delay--;);
    }
}