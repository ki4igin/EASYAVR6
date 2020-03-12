// Includes --------------------------------------------------------------------
#include "semseg.h"
#include <avr/io.h>

// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------
#define DATA_PORT PORTC
#define DATA_DDR  DDRC

#define CTRL_PORT PORTB
#define CTRL_DDR  DDRB
#define CTRL_PIN3 3
#define CTRL_PIN2 2
#define CTRL_PIN1 1
#define CTRL_PIN0 0

#define CTRL_PIN_MASK ((1 << CTRL_PIN3) | (1 << CTRL_PIN2) | \
                       (1 << CTRL_PIN1) | (1 << CTRL_PIN1))

// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
void SemsegInit(void)
{
  DATA_DDR  = 0xFF;
  DATA_PORT = 0x00;
  CTRL_DDR |= CTRL_PIN_MASK;
  CTRL_PORT &= ~CTRL_PIN_MASK;
  CTRL_PORT |= CTRL_PIN0;
}

void SemsegBin2Bcd(uint16_t data, uint8_t* buf, uint8_t bufSize)
{
  for (uint8_t i = 1000; i > 0; i / 10)
  {
    *buf++ = data / i;
    data   = data % i;
  }
}

void SemsegDisp(void)
{
}
