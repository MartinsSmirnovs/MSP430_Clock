#include "display.h"
#include "system.h"
#include "time.h"
#include <stdbool.h>

static void display( enum Symbol symbol );
static void indication( enum Symbol symbol, enum Position position );
static char getPositionCounter();
static void displayDivider();

static char getPositionCounter()
{
    static char counter = 0;
    counter++;
    if ( counter == 5 )
    {
        counter = 0;
    }

    return counter;
}

void indicationDouble( char digitsOneTwo, char digitsThreeFour )
{
    // Since buzzer control pins are connected to display position
    // control pins, we have to disable display during playing of sound
    // We have to make sure at the very top level that we do not display
    // anything during playing of sound. Failure to do so will result either
    // in bad audio output, or incorrect display
    if ( ( P2OUT & BuzzerPins ) == BuzzerPins )
    {
        P1OUT = 0;
        return;
    }

    const char counter = getPositionCounter();

    if ( counter <= 1 && digitsOneTwo == -1 || counter >= 2 && counter <= 3 && digitsThreeFour == -1 )
    {
        P2OUT &= ~0x0F;
        return;
    }

    switch ( counter )
    {
        case 0:
        {
            indication( ( enum Symbol )( ( digitsOneTwo % 100 ) / 10 ), PositionFirst );
        }
        break;
        case 1:
        {
            indication( ( enum Symbol )( digitsOneTwo % 10 ), PositionSecond );
        }
        break;
        case 2:
        {
            indication( ( enum Symbol )( ( digitsThreeFour % 100 ) / 10 ), PositionThird );
        }
        break;
        case 3:
        {
            indication( ( enum Symbol )( digitsThreeFour % 10 ), PositionFourth );
        }
        break;
        case 4:
        {
            displayDivider();
        }
        break;

        default:
            break;
    }
}

void displayDivider()
{
    static const int millisHalf = 500;
    const bool enableDivider = getTime().millis - millisHalf > 0;
    if ( enableDivider )
    {
        indication( Symbol_dot, PositionSecond );
    }
    else
    {
        P2OUT &= ~0x0F;
    }
}

void indication( enum Symbol symbol, enum Position position )
{
    P2OUT &= ~0x0F;
    display( symbol );
    P2OUT |= position;
}

void indicationQuad( enum Symbol one, enum Symbol two, enum Symbol three, enum Symbol four )
{
    if ( ( P2OUT & BuzzerPins ) == BuzzerPins )
    {
        P1OUT = 0;
        return;
    }

    const char counter = getPositionCounter();

    switch ( counter )
    {
        case 0:
        {
            indication( one, PositionFirst );
        }
        break;
        case 1:
        {
            indication( two, PositionSecond );
        }
        break;
        case 2:
        {
            indication( three, PositionThird );
        }
        break;
        case 3:
        {
            indication( four, PositionFourth );
        }
        break;
        case 4:
        {
            displayDivider();
        }
        break;

        default:
            break;
    }
}

struct SymbolValuePair
{
    enum Symbol symbol;
    unsigned char value;
};

static const struct SymbolValuePair symbolValuePairList[] = {
    { Symbol_0, 0xF5 },
    { Symbol_1, 0x05 },
    { Symbol_2, 0x73 },
    { Symbol_3, 0x57 },
    { Symbol_4, 0x87 },
    { Symbol_5, 0xD6 },
    { Symbol_6, 0xF6 },
    { Symbol_7, 0x45 },
    { Symbol_8, 0xF7 },
    { Symbol_9, 0xD7 },
    { Symbol_A, 0xE7 },
    { Symbol_L, 0xB0 },
    { Symbol_o, 0x36 },
    { Symbol_F, 0xE2 },
    { Symbol_n, 0x26 },
    { Symbol_dot, 0x08 },
    { Symbol_None, 0x00 },
};

static const unsigned char S_SymbolValuePairList = sizeof( symbolValuePairList ) / sizeof( symbolValuePairList[ 0 ] );

void display( enum Symbol symbol )
{
    for ( int i = 0; i < S_SymbolValuePairList; i++ )
    {
        if ( symbolValuePairList[ i ].symbol == symbol )
        {
            P1OUT = symbolValuePairList[ i ].value;
            return;
        }
    }

    P1OUT = 0;
}