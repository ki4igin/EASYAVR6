// Includes --------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>

// Typedef ---------------------------------------------------------------------
typedef enum
{
    STATE_TIME,
    STATE_TIME_SHOW,
    STATE_TIME_SET,
    STATE_TEMP,
    STATE_TEMP_SHOW,
    STATE_HELP,
    STATE_HELP_SHOW
} State_t;

typedef enum
{
    EVENT_UP,
    EVENT_BACK,
    EVENT_SET,
    EVENT_DOWN
} Event_t;

// Variables -------------------------------------------------------------------
State_t state;
Event_t event;

uint8_t btnOn = 0;

// Functions -------------------------------------------------------------------
int main(void)
{
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);
    sei();

    while (1)
    {
    }
}

ISR(TIMER2_OVF_vect)
{
    if ((btnOn == 0) && (PINA & 0x0F))
    {
        btnOn = 1;
        event = (Event_t)PINA;
    }
    else if ((btnOn == 1) && ((PINA & 0x0F) == 0))
    {
        btnOn = 0;
    }
    
    
}
// End File --------------------------------------------------------------------
