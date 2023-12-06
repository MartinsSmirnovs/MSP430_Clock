#include "event.h"
#include "system.h"
#include "time.h"
#include <msp430g2202.h>

enum Event bufferedEvent = EventNone;

void bufferEvent( enum Event event )
{
    bufferedEvent = event;
}

enum Event getBufferedEvent()
{
    enum Event currentEvent = bufferedEvent;
    bufferedEvent = EventNone;
    return currentEvent;
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_ISR()
{
    if ( P2IFG & ButtonTop )
    {
        P2IFG &= ~ButtonTop;
        bufferEvent( EventButtonTopPressed );
    }
    if ( P2IFG & ButtonBottom )
    {
        P2IFG &= ~ButtonBottom;

        static unsigned long long seconds = 0;
        if ( P2IES & ButtonBottom )
        {
            P2IES &= ~ButtonBottom;
            seconds = getSteadyTimeSeconds();
        }
        else
        {
            P2IES |= ButtonBottom;
            if ( getSteadyTimeSeconds() - seconds > 1 )
            {
                bufferEvent( EventButtonBottomPressedLong );
            }
            else
            {
                bufferEvent( EventButtonBottomPressed );
            }
        }
    }
}