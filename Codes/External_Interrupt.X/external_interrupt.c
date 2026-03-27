#include <xc.h>
#include "external_interrupt.h"

void init_external_interrupt(void)
{
	INTEDG0 = 1;
	INT0IF = 0;
	INT0IE = 1;
}