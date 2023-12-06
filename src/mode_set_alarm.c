#include "mode_set_alarm.h"
#include "display.h"
#include "event.h"
#include "mode.h"
#include "mode_alarm.h"
#include "time.h"
#include <stdbool.h>


static struct Time tmpAlarmTime = { 0, 0, 0, 0 };
static struct Time setAlarmTimeInitial = { 0, 0, 0, 0 };

void processModeSetAlarmClockPre()
{
    struct Time alarmTime = getAlarmTime();

    tmpAlarmTime.seconds = 0;
    tmpAlarmTime.minutes = alarmTime.minutes;
    tmpAlarmTime.hours = alarmTime.hours;

    setAlarmTimeInitial.minutes = alarmTime.minutes;
    setAlarmTimeInitial.hours = alarmTime.hours;
}

void processModeSetAlarmClockPost()
{
    // Set new time only if it was actually changed
    if ( tmpAlarmTime.minutes != setAlarmTimeInitial.minutes ||
         tmpAlarmTime.hours != setAlarmTimeInitial.hours )
    {
        setAlarmTime( tmpAlarmTime );
    }
}

void processModeSetAlarmClock( enum Event currentEvent, enum Submode submode, struct Time time )
{
    static const int millisHalf = 500;
    const bool displayDigits = time.millis - millisHalf > 0;

    if ( submode == BlinkHours )
    {
        if ( currentEvent == EventButtonBottomPressed )
        {
            tmpAlarmTime.hours++;
            if ( tmpAlarmTime.hours == 24 )
            {
                tmpAlarmTime.hours = 0;
            }
        }

        if ( displayDigits )
        {
            indicationDouble( tmpAlarmTime.hours, tmpAlarmTime.minutes );
        }
        else
        {
            indicationDouble( -1, tmpAlarmTime.minutes );
        }
    }
    else if ( submode == BlinkMinutes )
    {
        if ( currentEvent == EventButtonBottomPressed )
        {
            tmpAlarmTime.minutes++;
            if ( tmpAlarmTime.minutes == 60 )
            {
                tmpAlarmTime.minutes = 0;
            }
        }

        if ( displayDigits )
        {
            indicationDouble( tmpAlarmTime.hours, tmpAlarmTime.minutes );
        }
        else
        {
            indicationDouble( tmpAlarmTime.hours, -1 );
        }
    }
    else if ( submode == AlarmStatus )
    {
        if ( currentEvent == EventButtonBottomPressed )
        {
            setAlarmEnabled( !isAlarmEnabled() );
        }

        if ( isAlarmEnabled() )
        {
            indicationQuad( Symbol_A, Symbol_L, Symbol_o, Symbol_n );
        }
        else
        {
            indicationQuad( Symbol_A, Symbol_L, Symbol_o, Symbol_F );
        }
    }
}