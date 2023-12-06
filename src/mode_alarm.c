#include "mode_alarm.h"
#include "display.h"
#include "event.h"
#include "melody.h"
#include "time.h"
#include <stdbool.h>

static struct Time alarmTime = { 0, 0, 0, 0 };
static bool alarmEnabled = false;

struct Time getAlarmTime()
{
    return alarmTime;
}

void setAlarmTime( struct Time newAlarmTime )
{
    alarmTime = newAlarmTime;
}

void processAlarm( struct Time time )
{
    // Blink the alarm time
    if ( time.seconds % 2 == 0 )
    {
        indicationDouble( alarmTime.hours, alarmTime.minutes );
    }
    else
    {
        indicationDouble( -1, -1 );
    }

    playMelody();
}

bool doTriggerAlarm( struct Time time )
{
    if ( !alarmEnabled )
    {
        return false;
    }

    if ( time.hours != alarmTime.hours ||
         time.minutes != alarmTime.minutes ||
         time.seconds != alarmTime.seconds )
    {
        return false;
    }

    return true;
}

bool isAlarmEnabled()
{
    return alarmEnabled;
}

void setAlarmEnabled( bool enabled )
{
    alarmEnabled = enabled;
}

void processModeAlarmPre()
{
    // Since we manually generate EventAlarm, it is important for this event to occur only
    // once during event loop. Otherwise the alarm will resetMelody multiple times
    // By disabling alarm clock during the alarm event, and then enabling it back afterwards
    // we can guarantee the appearance of Alarm event only once
    alarmEnabled = false;

    resetMelody();
}

void processModeAlarmPost()
{
    alarmEnabled = true;

    disableMelody();
}