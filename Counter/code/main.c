// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "main.h"
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
  // Инициализация портов
  // PA0...PA7 - входы c PullUp
  DDRA  = 0x00;
  PORTA = 0xFF;
  // Инициализация спящего режима Power Down
  MCUCR &= ~((1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE));
  MCUCR |= (1 << SM1) | (1 << SM0);
  // Инициализация cемисегментных индикаторов
  // SemsegInit();
  // Основной цикл
  uint8_t buf[4];
  while (1)
  {
    if ((PINA & (1 << PINA1)) && !flag.btnOn)
    {
      flag.btnOn = 1;
      count      = (count < 9999) ? count + 1 : 0;
    }
    else
    {
      flag.btnOn = 0;
    }
    _delay_us(1000);
    SemsegBin2Bcd(count, buf, sizeof(buf));
    PORTA = buf[3];
    //SemsegDisp();
  }
}