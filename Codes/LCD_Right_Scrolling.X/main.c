#include <xc.h>
#include "clcd.h"
#define XTAL 8000000

static void init_config(void) {
    init_clcd();

}

//void main(void) {
//    init_config();
//    char str[20] = "Hello World     ";
//
//    int count = 0;
//    while (1) {
//        clcd_print(str, LINE1(0));
//
//        if (count++ == 1000) {
//            count = 0;
//            char last = str[15];
//            for (int i = 15; i > 0; i--) {
//                str[i] = str[i - 1];
//            }
//            str[0] = last;
//        }
//    }
//}

void main(void) {
    init_config();

    char str[17] = "Hello World     ";

    while (1) {
        clcd_print(str, LINE1(0));

        __delay_ms(2000);   // 0.5 Hz (2 sec delay)

        // Right scroll
        char last = str[15];

        for (int i = 15; i > 0; i--) {
            str[i] = str[i - 1];
        }

        str[0] = last;
    }
}