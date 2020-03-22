// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "semseg.h"

// Private Typedef -------------------------------------------------------------
// Тип пользовательских флагов
typedef struct
{
    uint8_t btnOn : 1,  // Флаг нажатия кнопки
        ledDir : 1;
} Flags_t;

// Private Variables -----------------------------------------------------------
extern uint8_t timerOcr;

Flags_t flag   = {0};  // Переменная пользовательских флагов
uint8_t buf[4] = {0};  // Буфер для хранения 4-х разрядного числа для
                       // вывода на семисегментные индикаторы

// Handlers --------------------------------------------------------------------
// Timer2 Overflow Handler
ISR(TIMER2_OVF_vect)
{
    /* Если нажата кнопка PВ6, то переключаем отображаемый байт данных на
    индикаторах;
    иначе, если нажата кнопка PB7 отправляем данные по UART;
    иначе, сбрасываем флаг нажатия кнопки
    */
    if (!(PINB & (1 << PINB6)))
    {
        if (!flag.btnOn)
        {
            flag.btnOn = 1;
            numByte    = (numByte < 4) ? numByte + 1 : 0;
        }
    }
    else if (!(PINB & (1 << PINB7)))
    {
        if (!flag.btnOn)
        {
            flag.btnOn = 1;
            flag.tx    = 1;
        }
    }
    else
    {
        flag.btnOn = 0;
    }
}

// IRQ1 Handler
ISR(TIMER0_OVF_vect)
{
    TimerInc();

    uint8_t buf[4];
    uint8_t kd = OCR1BL * 100 / 256;

    SemsegBin2Bcd(kd, buf, sizeof(buf));
    SemsegDisp(buf, sizeof(buf));
}

// Timer Compare Handler
void TimerComp(void)
{
    if (flag.ledDir)
    {
        PORTA = (PORTA == 0) ? 0x80 : PORTA >> 1;
    }
    else
    {
        PORTA = (PORTA == 0) ? 0x01 : PORTA << 1;
    }
}
// End File --------------------------------------------------------------------