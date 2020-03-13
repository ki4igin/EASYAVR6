// Includes --------------------------------------------------------------------
#include "semseg.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

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
                       (1 << CTRL_PIN1) | (1 << CTRL_PIN0))

// Private Constants -----------------------------------------------------------
const uint8_t tableDecoder[10] PROGMEM = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110};
//uint8_t tableDecoder[10] = {
//1,
//2,
//3,
//4,
//5,
//6,
//7,
//8,
//9,
//10
//};
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
  buf += bufSize - 1;
  // for (uint16_t i = 1000; i > 0; i /= 10)
  // {
  //   *buf-- = data / i;
  //   data   = data % i;
  // }
  *buf-- = data / 1000;
  data %= 1000;
  *buf-- = data / 100;
  data %= 100;
  *buf-- = data / 10;
  data %= 10;
  *buf-- = data / 1;
  data %= 1;
}

void SemsegDisp(uint8_t* buf, uint8_t bufSize)
{
  uint8_t ind = 0;

  DATA_PORT = 0x00;
  if (CTRL_PORT & (1 << CTRL_PIN0))
  {
    CTRL_PORT &= ~(1 << CTRL_PIN0);
    CTRL_PORT |= (1 << CTRL_PIN1);
    ind  = buf[1];
  }
  else if (CTRL_PORT & (1 << CTRL_PIN1))
  {
    CTRL_PORT &= ~(1 << CTRL_PIN1);
    CTRL_PORT |= (1 << CTRL_PIN2);
    ind  = buf[2];
  }   
  else if (CTRL_PORT & (1 << CTRL_PIN2))
  {
    CTRL_PORT &= ~(1 << CTRL_PIN2);
    CTRL_PORT |= (1 << CTRL_PIN3);
    ind  = buf[3];
  }   
  else if (CTRL_PORT & (1 << CTRL_PIN3))
  {
    CTRL_PORT &= ~(1 << CTRL_PIN3);
    CTRL_PORT |= (1 << CTRL_PIN0);
    ind  = buf[0];
  }   
  else
  {
    return;   
  }
  uint8_t temp = pgm_read_byte(&tableDecoder[ind]);
  DATA_PORT    = temp;
}
