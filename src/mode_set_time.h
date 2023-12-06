#ifndef MODE_SET_TIME_H
#define MODE_SET_TIME_H

#include "mode.h"
#include "time.h"

void processModeSetTimePre();
void processModeSetTimePost();
void processModeSetTime( enum Event currentEvent, enum Submode submode, struct Time time );

#endif
