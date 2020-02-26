#define __AVR_ATmega16__
#define F_CPU 8000000
// Includes --------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "main.h"
// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------
#define LED_PORT PORTC
#define LED_PIN  PIN7
// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
int main(void)
{
  // Иницилизация портов
  // PB0...PB4, PB7 - входы; PB5, PB6 - выходы
  // Входы - PullUp, выходы - лог. 0
  DDRB = (1 << DDB5) | (1 << DDB6);
  PORTB &= ~((1 << PORTB5) | (1 << PORTB6));

  // Основной цикл
  while (1)
  {
    PORTB |= 1 << PORTB5;     // Устанваливаем PB5
    PORTB &= ~(1 << PORTB5);  // Сбрасываем PB5
  }
}

ISR(INT1_vect)
{
  PORTC ^= (1 << PC0);  //Инвертируем состояние светодиода
}

ISR(TIMER2_COMP_vect)
{
}

void EepromWrite(uint16_t adr, uint8_t data)
{
  while (EECR & (1 << EEWE))  // Ждем окончания предыдущей записи
  {
    ;
  }
  EEAR = adr;   // Указываем адрес для записи
  EEDR = data;  // Указываем данные для записи
  // cli();                 // Запрещаем прерывания
  EECR |= (1 << EEMWE);  // Разрешаем запиcь в EEPROM на 4 такта
  EECR |= (1 << EEWE);   // Записываем данные в EEPROM
  // sei();                 // Разрешаем прерывания
}

uint8_t EepromRead(uint16_t adr)
{
  while (EECR & (1 << EEWE))  // Ждем окончания предыдущей записи
  {
    ;
  }
  EEAR = adr;           // Указываем адрес для чтения
  EECR |= (1 << EERE);  // Считываем данные из EEPROM
  return EEDR;          // Возврашаем считанные данне
}
