#ifndef TIME_H
#define TIME_H

struct Time
{
    char seconds;
    char minutes;
    char hours;
    int millis;
};

void setTime( struct Time newTime );
struct Time getTime();

unsigned long long getSteadyTimeSeconds();

#endif