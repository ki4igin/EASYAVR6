// Includes --------------------------------------------------------------------
#include "main.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "semseg.h"
// Private Typedef -------------------------------------------------------------
typedef struct
{
  uint8_t btnOn : 1;
} Flags_t;

// Private Macro ---------------------------------------------------------------
#define LED_PORT PORTC
#define LED_PIN  PIN7
// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------
void Tim0Init(void);
// Functions -------------------------------------------------------------------
int main(void)
{
  Flags_t  flag  = {0};
  uint16_t count = {0};
  uint8_t buf[4];

  // Инициализация портов
  // PA0...PA7 - входы c PullUp
  DDRA  = 0x00;
  PORTA = 0xFF;
  // Инициализация спящего режима Power Down
  MCUCR &= ~((1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE));
  MCUCR |= (1 << SM1) | (1 << SM0);
  // Инициализация cемисегментных индикаторов
  SemsegInit();
  SemsegOff();
  // Основной цикл
  while (1)
  {
    if (PINA & (1 << PINA0))
    {
      flag.btnOn = 0;
    }
    else
    {
      if (!flag.btnOn)
      {
        flag.btnOn = 1;
        count      = (count < 9999) ? count + 1 : 0;
      }
    }
    _delay_us(1000);    
    SemsegBin2Bcd(count, buf, sizeof(buf));
    SemsegDisp(buf, sizeof(buf));
  }
}
// End File --------------------------------------------------------------------