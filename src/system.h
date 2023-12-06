#ifndef SYSTEM_H
#define SYSTEM_H

#include <msp430g2202.h>

enum
{
    ButtonTop = BIT5,
    ButtonBottom = BIT4,
    ButtonPins = ButtonBottom | ButtonTop,
    ButtonLongPress,
    BuzzerPins = BIT3 | BIT2
};

void delay( unsigned long millis );

#endif