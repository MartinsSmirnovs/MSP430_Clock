#include "display.h"
#include "event.h"
#include "mode.h"
#include "mode_alarm.h"
#include "mode_set_alarm.h"
#include "mode_set_time.h"
#include "system.h"
#include "time.h"
#include <msp430g2202.h>
#include <stdbool.h>

void processModeShowMinutesSeconds( struct Time time );
void processModeShowHoursMinutes( struct Time time );

void setup()
{
    WDTCTL = WDTPW + WDTHOLD; // disable watchdog

    TACTL = TASSEL_2 + MC_1 + ID_0 + TACLR;
    TACCTL0 = CCIE;
    CCR0 = 32; // A value to count to, to get one millisecond
    BCSCTL2 = SELM_0 + DIVM_0 + SELS + DIVS_0;
    BCSCTL1 &= ~XTS;
    BCSCTL3 = ( XT2S_0 + XCAP_3 );

    BCSCTL1 = CALBC1_12MHZ; //set timer frequency 12MHz
    DCOCTL = CALDCO_12MHZ;  //set timer frequency 12MHz

    P2DIR = PositionFirst | PositionSecond | PositionThird | PositionFourth | BuzzerPins;
    P1DIR = 0xFF;
    P2OUT = 0;
    P1OUT = 0;
    // flags for ButtonPins before enabling the interrupt
    P2DIR &= ~ButtonPins;
    P2IES |= ButtonPins;  // interrupt on high->low
    P2IFG &= ~ButtonPins; //to prevent immediate interrupt, clear the
    P2IE |= ButtonPins;   //enable interrupt
    __enable_interrupt();
}

int main()
{
    setup();

    struct Mode mode = initialMode();

    while ( true )
    {
        const enum Event event = getBufferedEvent();

        getCurrentModePostCallback( event, *mode.main )();

        mode = updateMode( event, mode );

        getCurrentModePreCallback( event, *mode.main )();

        const struct Time time = getTime();
        if ( doTriggerAlarm( time ) )
        {
            bufferEvent( EventAlarm );
        }

        switch ( mode.main->modeType )
        {
            case ModeShowMinutesSeconds:
            {
                processModeShowMinutesSeconds( time );
            }
            break;
            case ModeShowHoursMinutes:
            {
                processModeShowHoursMinutes( time );
            }
            break;
            case ModeSetTime:
            {
                processModeSetTime( event, mode.sub, time );
            }
            break;
            case ModeSetAlarmClock:
            {
                processModeSetAlarmClock( event, mode.sub, time );
            }
            break;
            case ModeAlarm:
            {
                processAlarm( time );
            }
            break;
            default:
                break;
        }
    }
    return 0;
}

void processModeShowMinutesSeconds( struct Time time )
{
    indicationDouble( time.minutes, time.seconds );
}

void processModeShowHoursMinutes( struct Time time )
{
    indicationDouble( time.hours, time.minutes );
}