#include "system.h"
#include <msp430g2202.h>

void delay( unsigned long millis )
{
    for ( unsigned long i = 0; i < millis; i++ )
    {
        __delay_cycles( 120 );
    }
}
