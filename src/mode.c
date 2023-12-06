#include "mode.h"
#include "event.h"
#include "mode_alarm.h"
#include "mode_set_alarm.h"
#include "mode_set_time.h"

static void doNothing()
{
}

extern const struct MainModeDescription modeShowMinutesSecondsDescription;
extern const struct MainModeDescription modeShowHoursMinutesDescription;
extern const struct MainModeDescription modeSetTimeDescription;
extern const struct MainModeDescription modeSetAlarmClockDescription;
extern const struct MainModeDescription modeAlarmDescription;

const struct MainModeDescription modeShowMinutesSecondsDescription = {
    ModeShowMinutesSeconds, doNothing, doNothing, &modeShowHoursMinutesDescription
};

const struct MainModeDescription modeShowHoursMinutesDescription = {
    ModeShowHoursMinutes, doNothing, doNothing, &modeSetTimeDescription
};

const struct MainModeDescription modeSetTimeDescription = {
    ModeSetTime, processModeSetTimePre, processModeSetTimePost, &modeSetAlarmClockDescription
};

const struct MainModeDescription modeSetAlarmClockDescription = {
    ModeSetAlarmClock, processModeSetAlarmClockPre, processModeSetAlarmClockPost, &modeShowMinutesSecondsDescription
};

const struct MainModeDescription modeAlarmDescription = {
    ModeAlarm, processModeAlarmPre, processModeAlarmPost, 0
};

struct Mode initialMode()
{
    struct Mode mode = { &modeShowMinutesSecondsDescription, BlinkHours };
    return mode;
}

static enum Submode getNextSubmode( struct Mode currentMode )
{
    switch ( currentMode.main->modeType )
    {
        case ModeSetAlarmClock:
        {
            if ( currentMode.sub == AlarmStatus )
            {
                return BlinkHours;
            }

            if ( currentMode.sub == BlinkHours )
            {
                return BlinkMinutes;
            }

            if ( currentMode.sub == BlinkMinutes )
            {
                return AlarmStatus;
            }
        }
        break;
        default:
        {
            if ( currentMode.sub == BlinkHours )
            {
                return BlinkMinutes;
            }

            if ( currentMode.sub == BlinkMinutes )
            {
                return BlinkHours;
            }
        }
        break;
    }
}

struct Mode updateMode( enum Event currentEvent, struct Mode currentMode )
{
    switch ( currentEvent )
    {
        case EventButtonTopPressed:
        {
            static struct Mode currentModeBackup;
            if ( currentMode.main->modeType == ModeAlarm )
            {
                // BUG: This will trigger an execution of current mode pre-setup function which
                // will delete data if the user was in Clock Set or Alarm Clock set
                // modes during the ringing of alarm
                return currentModeBackup;
            }

            currentMode.main = currentMode.main->nextMode;
            currentModeBackup = currentMode;

            if ( currentMode.main->modeType == ModeSetAlarmClock )
            {
                currentMode.sub = AlarmStatus;
            }
            else
            {
                currentMode.sub = BlinkHours;
            }
        }
        break;
        case EventButtonBottomPressedLong:
        {
            currentMode.sub = getNextSubmode( currentMode );
        }
        break;

        case EventAlarm:
        {
            currentMode.main = &modeAlarmDescription;
        }
        break;

        case EventButtonBottomPressed:
        case EventNone:
        default:
            break;
    }

    return currentMode;
}

void ( *getCurrentModePostCallback( enum Event currentEvent, struct MainModeDescription currentMode ) )()
{
    // Return an actual function only when changing modes or on special events
    if ( currentEvent == EventNone ||
         currentEvent == EventButtonBottomPressed ||
         currentEvent == EventButtonBottomPressedLong )
    {
        return doNothing;
    }

    return currentMode.postCallback;
}

void ( *getCurrentModePreCallback( enum Event currentEvent, struct MainModeDescription currentMode ) )()
{
    // Return an actual function only when changing modes or on special events
    if ( currentEvent == EventNone ||
         currentEvent == EventButtonBottomPressed ||
         currentEvent == EventButtonBottomPressedLong )
    {
        return doNothing;
    }

    return currentMode.preCallback;
}