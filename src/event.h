#ifndef EVENT_H
#define EVENT_H

enum Event
{
    EventNone = 0,
    EventButtonTopPressed,
    EventButtonBottomPressed,
    EventButtonBottomPressedLong,
    EventAlarm
};

// Buffers passed event for processing later
void bufferEvent( enum Event event );

// The main function to get current event from
// This function must be called only ONCE during each event loop iteration
// After calling this function, the event counts as "processed", and further
// calls to this function will return EventNone (if no further events occurred)
//
enum Event getBufferedEvent();

#endif