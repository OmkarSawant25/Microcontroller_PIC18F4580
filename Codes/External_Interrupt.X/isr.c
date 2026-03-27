#include <xc.h>

unsigned char  key_detected;

void __interrupt() isr(void)
{
	if (INT0IF == 1)
	{
		key_detected = 1;

		INT0IF = 0;
	}
}