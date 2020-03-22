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
    uint8_t btnOn : 1;  // Флаг нажатия кнопки
    uint8_t ledDir : 1;
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
    /* Если нажата кнопка PD0, то увеличиваем коэффициет заполнения ШИМ;
    иначе, если нажата кнопка PD1, то уменьшаем коэффициет заполнения ШИМ;    
    иначе, если нажата кнопка PD5, то изменяем направление "бегущего огонька";
    иначе, если нажата кнопка PD6, то увеличиваем скорость "бегущего огонька";
    иначе, если нажата кнопка PD7, то уменьшаем скорость "бегущего огонька";
    иначе, сбрасываем флаг нажатия кнопки
    */
    if (!(PIND & (1 << PIND0)))
    {
        OCR1BL++;
    }
    else if (!(PIND & (1 << PIND1)))
    {
        OCR1BL--;
    }
    else if (!(PIND & (1 << PIND5)))
    {
        if (flag.btnOn)
        {
            ;
        }
        else
        {
            flag.btnOn  = 1;
            flag.ledDir = !flag.ledDir;
        }
    }
    else if (!(PIND & (1 << PIND6)))
    {
        timerOcr--;
    }
    else if (!(PIND & (1 << PIND7)))
    {
        timerOcr++;
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