#ifndef COMMONS_H
#define COMMONS_H

#define F_CPU 8000000
#include <util/delay.h>

#define nop() asm("nop\t\n")
#define soft_reset() ((void (*)(void)) 0x0000)()

#endif // COMMONS_H
