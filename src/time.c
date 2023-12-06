#include "time.h"

static void timeEngine( char millisNow );

struct Time time = { 0, 0, 0, 0 };
unsigned long long steadyTimeSeconds = 0;

unsigned long long getSteadyTimeSeconds()
{
    return steadyTimeSeconds;
}

struct Time getTime()
{
    return time;
}

void setTime( struct Time newTime )
{
    time.hours = newTime.hours;
    time.minutes = newTime.minutes;
    time.seconds = newTime.seconds;
    time.millis = newTime.millis;
}


void timeEngine( char millisNow )
{
    time.millis += millisNow;
    if ( time.millis == 1000 )
    {
        time.millis = 0;
        time.seconds++;
        steadyTimeSeconds++;
    }

    if ( time.seconds == 60 )
    {
        time.seconds = 0;
        time.minutes++;
    }

    if ( time.minutes == 60 )
    {
        time.minutes = 0;
        time.hours++;
    }

    if ( time.hours == 24 )
    {
        time.hours = 0;
    }
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A()
{
    timeEngine( 1 );
}