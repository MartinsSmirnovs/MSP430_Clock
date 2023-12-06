#ifndef MODE_SET_ALARM_H
#define MODE_SET_ALARM_H

#include "event.h"
#include "mode.h"
#include "time.h"

void processModeSetAlarmClockPre();
void processModeSetAlarmClockPost();
void processModeSetAlarmClock( enum Event currentEvent, enum Submode mode, struct Time time );

#endif
