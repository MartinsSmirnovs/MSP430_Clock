#ifndef MODE_ALARM_H
#define MODE_ALARM_H

#include "time.h"
#include <stdbool.h>

void processAlarm( struct Time time );
bool doTriggerAlarm( struct Time time );

void setAlarmTime( struct Time newAlarmTime );
struct Time getAlarmTime();

bool isAlarmEnabled();
void setAlarmEnabled( bool enabled );

void processModeAlarmPre();
void processModeAlarmPost();

#endif