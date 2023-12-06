#include "mode_set_time.h"
#include "display.h"
#include "mode.h"
#include "time.h"
#include <stdbool.h>

static struct Time tmpSetTime = { 0, 0, 0, 0 };
static struct Time setTimeInitial = { 0, 0, 0, 0 };

void processModeSetTimePre()
{
    const struct Time time = getTime();
    tmpSetTime.minutes = time.minutes;
    tmpSetTime.hours = time.hours;

    setTimeInitial.minutes = time.minutes;
    setTimeInitial.hours = time.hours;
}

void processModeSetTimePost()
{
    // Set new time only if it was actually changed
    if ( tmpSetTime.minutes != setTimeInitial.minutes ||
         tmpSetTime.hours != setTimeInitial.hours )
    {
        setTime( tmpSetTime );
    }
}

void processModeSetTime( enum Event currentEvent, enum Submode submode, struct Time time )
{
    static const int millisHalf = 500;
    const bool displayDigits = time.millis - millisHalf > 0;

    if ( submode == BlinkHours )
    {
        if ( currentEvent == EventButtonBottomPressed )
        {
            tmpSetTime.hours++;
            if ( tmpSetTime.hours == 24 )
            {
                tmpSetTime.hours = 0;
            }
        }

        if ( displayDigits )
        {
            indicationDouble( tmpSetTime.hours, tmpSetTime.minutes );
        }
        else
        {
            indicationDouble( -1, tmpSetTime.minutes );
        }
    }
    else if ( submode == BlinkMinutes )
    {
        if ( currentEvent == EventButtonBottomPressed )
        {
            tmpSetTime.minutes++;
            if ( tmpSetTime.minutes == 60 )
            {
                tmpSetTime.minutes = 0;
            }
        }

        if ( displayDigits )
        {
            indicationDouble( tmpSetTime.hours, tmpSetTime.minutes );
        }
        else
        {
            indicationDouble( tmpSetTime.hours, -1 );
        }
    }
}
