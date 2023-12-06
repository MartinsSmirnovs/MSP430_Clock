#include "melody.h"
#include "system.h"
#include <msp430g2202.h>
#include <stdbool.h>

int melodyIterator = 0;
long currentNotePlaytime = 0;

enum Note
{
    Note_A = 28,
    Note_B = 25,
    Note_C = 47,
    Note_D = 42,
    Note_E = 37,
    Note_F = 35,
    Note_G = 31,
    Note_Pause = 50
};

enum NoteDuration
{
    NoteDuration_Multiplier = 600,
    NoteDuration_Whole = 100,
    NoteDuration_Half = 50,
    NoteDuration_Quarter = 25,
    NoteDuration_Short = 3
};

struct NoteDurationPair
{
    enum Note note;
    enum NoteDuration duration;
};

static const struct NoteDurationPair melody[] = {
    { Note_A, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_C, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_D, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_D, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_D, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_E, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_F, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_F, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_F, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_G, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_E, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_E, NoteDuration_Half },
    { Note_Pause, NoteDuration_Short },

    { Note_D, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_C, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_C, NoteDuration_Quarter },
    { Note_Pause, NoteDuration_Short },

    { Note_D, NoteDuration_Half },
    { Note_Pause, NoteDuration_Half },
};

static const int S_Melody = sizeof( melody ) / sizeof( melody[ 0 ] );

static bool play( struct NoteDurationPair pair );

void resetMelody()
{
    melodyIterator = 0;
    currentNotePlaytime = 0;
}

void disableMelody()
{
    // Make sure the buzzer is turned off in the end
    P2OUT &= ~BuzzerPins;
}

void playMelody()
{
    const bool isNextNote = play( melody[ melodyIterator ] );

    if ( isNextNote )
    {
        melodyIterator++;

        if ( melodyIterator == S_Melody )
        {
            melodyIterator = 0;
        }
    }
}

bool play( struct NoteDurationPair pair )
{
    // to not to get into infinite loop, the pulse length must be non-zero
    const int pulseLength = pair.note != 0 ? pair.note : 1;

    const long outputLength = ( long )pair.duration * NoteDuration_Multiplier;

    // This delay is short enough to not to feel it's effects too bad
    delay( pulseLength );

    // If buzzer is enabled
    if ( ( P2OUT & BuzzerPins ) == BuzzerPins )
    {
        P2OUT &= ~BuzzerPins;
    }
    // We do not want to buzz during pause
    else if ( pair.note != Note_Pause )
    {
        // Since buzzer control pins are connected to display position
        // control pins, we have to disable display during playing of sound
        // Otherwise we will show trash values on display
        P1OUT = 0;
        P2OUT |= BuzzerPins;
    }

    currentNotePlaytime += pulseLength;

    if ( currentNotePlaytime >= outputLength )
    {
        currentNotePlaytime = 0;
        return true;
    }

    return false;
}