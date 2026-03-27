/*
 * File:   main.c
 * Author: OMKAR SAWANT
 * Program: PWM based LED brightness control using Timer0 ISR
 * Initial brightness = 50% duty cycle
 * On DKS1 press -> brightness becomes 100% for 5 seconds
 * After 5 seconds -> returns to 50% brightness
 */

#include <xc.h>

#define LEVEL 0
#define EDGE  1

// Digital Keypad Switch 1 value
#define SW1   0x0E

// PWM parameters
unsigned int duty_cycle = 50; // Initial duty cycle = 50% (dim)
unsigned int period = 100; // Total PWM period

// Variables used in ISR
unsigned int pwm_count = 0; // PWM counter
unsigned int count = 0; // Counter for time calculation
unsigned int sec = 0; // Seconds counter

char once = 1; // Flag for edge detection

// Function prototypes
char scan_switch(char detect_type);
void __interrupt() ISR();

void main(void) {

    TRISBbits.TRISB0 = 0; // Configure RB0 as output (LED)
    TRISC = TRISC | 0x0F; // Configure lower 4 bits of PORTC as input (DKS)

    PORTBbits.RB0 = 0; // Initially LED OFF

    // Timer0 configuration
    T0CONbits.T08BIT = 1; // 8-bit mode
    T0CONbits.T0CS = 0; // Internal clock (instruction cycle)
    T0CONbits.PSA = 1; // No prescaler assigned

    TMR0 = 0; // Initialize Timer0
    T0CONbits.TMR0ON = 1; // Start Timer0

    // Enable interrupts
    INTCONbits.GIE = 1; // Global interrupt enable
    INTCONbits.PEIE = 1; // Peripheral interrupt enable
    INTCONbits.TMR0IE = 1; // Timer0 interrupt enable
    INTCONbits.TMR0IF = 0; // Clear Timer0 interrupt flag

    char key;

    while (1) {

        // Scan keypad using edge detection
        key = scan_switch(EDGE);

        // If switch is pressed
        if (key == SW1) {
            duty_cycle = 100; // Set LED to full brightness
            sec = 0; // Reset 5-second timer
        }

        // After 5 seconds
        if (sec >= 5) {
            duty_cycle = 50; // Return to dim brightness
            sec = 0; // Reset timer
        }
    }
}

// Function to scan Digital Keypad Switch

char scan_switch(char detect_type) {

    if (detect_type == LEVEL) {
        // Return current switch status
        return PORTC & 0x0F;
    }
    else if (detect_type == EDGE) {
        // Detect key press only once (edge detection)
        if ((PORTC & 0x0F) != 0x0F && once) {
            once = 0; // Disable further detection
            return PORTC & 0x0F; // Return pressed key
        }
        else if ((PORTC & 0x0F) == 0x0F) {
            once = 1; // Reset flag when key released
        }
    }

    return 0x0F; // No key pressed
}

// Interrupt Service Routine

void __interrupt() ISR() {

    // Check if Timer0 interrupt occurred
    if (INTCONbits.TMR0IF) {

        INTCONbits.TMR0IF = 0; // Clear interrupt flag
        TMR0 = TMR0 + 8; // Reload timer for consistent timing

        // PWM Generation
        pwm_count++; // Increment PWM counter

        if (pwm_count < duty_cycle) {
            PORTBbits.RB0 = 1; // LED ON
        }
        else if (pwm_count < period) {
            PORTBbits.RB0 = 0; // LED OFF
        }
        else {
            pwm_count = 0; // Reset PWM cycle
        }

        // Time Calculation
        if (++count >= 20000) { // Approximate 1 second delay
            count = 0;
            sec++; // Increment seconds
        }
    }
}