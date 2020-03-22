// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "semseg.h"

// Private Typedef -------------------------------------------------------------
// ��� ���������������� ������
typedef struct
{
    uint8_t btnOn : 1;  // ���� ������� ������
    uint8_t ledDir : 1;
} Flags_t;

// Private Variables -----------------------------------------------------------
extern uint8_t timerOcr;

Flags_t flag   = {0};  // ���������� ���������������� ������
uint8_t buf[4] = {0};  // ����� ��� �������� 4-� ���������� ����� ���
                       // ������ �� �������������� ����������

// Handlers --------------------------------------------------------------------
// Timer2 Overflow Handler
ISR(TIMER2_OVF_vect)
{
    /* ���� ������ ������ PD0, �� ����������� ���������� ���������� ���;
    �����, ���� ������ ������ PD1, �� ��������� ���������� ���������� ���;    
    �����, ���� ������ ������ PD5, �� �������� ����������� "�������� �������";
    �����, ���� ������ ������ PD6, �� ����������� �������� "�������� �������";
    �����, ���� ������ ������ PD7, �� ��������� �������� "�������� �������";
    �����, ���������� ���� ������� ������
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