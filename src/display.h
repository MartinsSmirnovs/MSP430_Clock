#ifndef DISPLAY_H
#define DISPLAY_H

#include <msp430g2202.h>

enum Position
{
    PositionFirst = BIT0,
    PositionSecond = BIT1,
    PositionThird = BIT2,
    PositionFourth = BIT3
};

enum Symbol
{
    Symbol_0 = 0,
    Symbol_1,
    Symbol_2,
    Symbol_3,
    Symbol_4,
    Symbol_5,
    Symbol_6,
    Symbol_7,
    Symbol_8,
    Symbol_9,
    Symbol_A,
    Symbol_L,
    Symbol_o,
    Symbol_F,
    Symbol_n,
    Symbol_dot,
    Symbol_None,
};

void indicationDouble( char digitsOneTwo, char digitsThreeFour );
void indicationQuad( enum Symbol one, enum Symbol two, enum Symbol three, enum Symbol four );

#endif