// Includes --------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"

// Typedef ---------------------------------------------------------------------
typedef enum
{
    STATE_TIME,
    STATE_TIME_SHOW,
    STATE_TIME_SET,
    STATE_TEMP,
    STATE_TEMP_SHOW,
    STATE_CNT
} State_t;

typedef enum
{
    EVENT_UP,
    EVENT_BACK,
    EVENT_SET,
    EVENT_DOWN,
    EVENT_NONE,
    EVENT_CNT
} Event_t;

// Variables -------------------------------------------------------------------
State_t state;
Event_t event;

uint8_t btnOn = 0;

State_t fsmStateTable[STATE_CNT][EVENT_CNT] = {
    [STATE_TIME][EVENT_UP]   = STATE_TEMP,
    [STATE_TIME][EVENT_DOWN] = STATE_TEMP,
    [STATE_TIME][EVENT_BACK] = STATE_TIME,
    [STATE_TIME][EVENT_SET]  = STATE_TEMP_SHOW,
    [STATE_TIME][EVENT_NONE] = STATE_TIME,

    [STATE_TIME_SHOW][EVENT_UP]   = STATE_TIME_SHOW,
    [STATE_TIME_SHOW][EVENT_DOWN] = STATE_TIME_SHOW,
    [STATE_TIME_SHOW][EVENT_BACK] = STATE_TIME,
    [STATE_TIME_SHOW][EVENT_SET]  = STATE_TIME_SET,
    [STATE_TIME_SHOW][EVENT_NONE] = STATE_TIME_SHOW,

    [STATE_TIME_SET][EVENT_UP]   = STATE_TIME_SHOW,
    [STATE_TIME_SET][EVENT_DOWN] = STATE_TIME_SHOW,
    [STATE_TIME_SET][EVENT_BACK] = STATE_TIME_SHOW,
    [STATE_TIME_SET][EVENT_SET]  = STATE_TIME_SHOW,
    [STATE_TIME_SET][EVENT_NONE] = STATE_TIME_SHOW,

    [STATE_TEMP][EVENT_UP]   = STATE_TIME,
    [STATE_TEMP][EVENT_DOWN] = STATE_TIME,
    [STATE_TEMP][EVENT_BACK] = STATE_TEMP,
    [STATE_TEMP][EVENT_SET]  = STATE_TEMP_SHOW,
    [STATE_TEMP][EVENT_NONE] = STATE_TEMP,

    [STATE_TEMP_SHOW][EVENT_UP]   = STATE_TEMP_SHOW,
    [STATE_TEMP_SHOW][EVENT_DOWN] = STATE_TEMP_SHOW,
    [STATE_TEMP_SHOW][EVENT_BACK] = STATE_TEMP,
    [STATE_TEMP_SHOW][EVENT_SET]  = STATE_TEMP_SHOW,
    [STATE_TEMP_SHOW][EVENT_NONE] = STATE_TEMP_SHOW,
};

void (*fsmActiomTable[STATE_CNT])() = {
    [STATE_TIME]      = Time,
    [STATE_TIME_SHOW] = TimeShow,
    [STATE_TIME_SET]  = TimeSet,
    [STATE_TEMP]      = Temp,
    [STATE_TEMP_SHOW] = TempShow,
};

// Functions -------------------------------------------------------------------
State_t FsmGetNewState(State_t state, Event_t event);
void    FsmAction(State_t state);

int main(void)
{
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);
    sei();

    while (1)
    {
        state = FsmGetNewState(state, event);
        FsmAction(state);
    }
}

State_t FsmGetNewState(State_t state, Event_t event)
{
    return fsmStateTable[state][event];
}

// void FsmAction(State_t state)
// {
//     fsmActiomTable[state]();
// }

void FsmAction(State_t state)
{
    switch (state)
    {
        case STATE_TIME:
            Time();
            break;
        case STATE_TIME_SHOW:
            TimeShow();
            break;
        case STATE_TIME_SET:
            TimeSet();
            break;
        case STATE_TEMP:
            Temp();
            break;
        case STATE_TEMP_SHOW:
            TempShow();
            break;
        default:
            break;
    }
}

ISR(TIMER2_OVF_vect)
{
    uint8_t pin = PINA & 0x0F;
    if (pin)
    {
        if (btnOn == 0)
        {
            btnOn = 0;
            if (pin & (1 << PA0))
            {
                event = EVENT_UP;
            }
            else if (pin & (1 << PA1))
            {
                event = EVENT_DOWN;
            }
            else if (pin & (1 << PA2))
            {
                event = EVENT_BACK;
            }
            else if (pin & (1 << PA3))
            {
                event = EVENT_SET;
            }
        }
    }
    else
    {
        btnOn = 0;
    }
}
// End File --------------------------------------------------------------------
