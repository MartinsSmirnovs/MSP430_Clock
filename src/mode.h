#ifndef MODE_H
#define MODE_H

#include "event.h"

enum MainMode
{
    ModeShowMinutesSeconds = 0,
    ModeShowHoursMinutes,
    ModeSetTime,
    ModeSetAlarmClock,
    ModeAlarm
};

enum Submode
{
    AlarmStatus = 0,
    BlinkHours,
    BlinkMinutes
};

struct MainModeDescription
{
    const enum MainMode modeType;
    void ( *preCallback )();
    void ( *postCallback )();
    const struct MainModeDescription* nextMode;
};

struct Mode
{
    const struct MainModeDescription* main;
    enum Submode sub;
};

struct Mode initialMode();
struct Mode updateMode( enum Event currentEvent, struct Mode currentMode );

void ( *getCurrentModePostCallback( enum Event currentEvent, struct MainModeDescription currentMode ) )();
void ( *getCurrentModePreCallback( enum Event currentEvent, struct MainModeDescription currentMode ) )();

#endif