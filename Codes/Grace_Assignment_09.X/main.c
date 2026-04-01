/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 31 March, 2026, 12:39 PM
 */

#include <xc.h>
#include "clcd.h"

#define _XTAL_FREQ 8000000

static void init_config(void) {
    init_clcd();
}

void main(void) {
    init_config();
    char str[] = "0000000000";
    char msg[] = "UP COUNTER";

    while (1) {
        str[9]++;

        if (str[9] == '9') {
            int i = 8;
            for (; i >= 0; i++) {
                if (str[i] != '9') {
                    str[i]++;
                    break;
                }
            }
            i++;
            for (; i < 10; i++) {
                str[i] = '0';
            }
        }

        // Method 2
        //        for (int i = 9; i >= 0; i--) {
        //            if (str[i] > '9') {
        //                str[i] = '0';
        //                if (i > 0) {
        //                    str[i - 1]++;
        //                }
        //            } else {
        //                break;
        //            }
        //        }

        clcd_print(msg, LINE1(0));
        clcd_print(str, LINE2(0));
        __delay_ms(500);

    }



    return;
}
