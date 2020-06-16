// Includes --------------------------------------------------------------------
#include <avr/io.h>         // ������������ ���� ��� ������ � ��� ��
#include <avr/interrupt.h>  // ������������ ���� ��� ������ � ������������ ��

#include "main.h"
#include "semseg.h"  // ������������ ���� ��� ������ � ��������������� ������������
#include "timer.h"   // ������������ ���� ��� ������ � ����������� ��������

// Handlers --------------------------------------------------------------------
/*******************************************************************************
Timer2 Overflow Handler
������������ ��� ��������� ������� ������
*******************************************************************************/
ISR(TIMER2_OVF_vect)
{
    // ���� ������ ������ PD0, �� ����������� ���������� ���������� ���;
    // �����, ���� ������ ������ PD1, �� ��������� ���������� ���������� ���;    
    // �����, ���� ������ ������ PB5, �� �������� ����������� "�������� �������";
    // �����, ���� ������ ������ PB6, �� ����������� �������� "�������� �������";
    // �����, ���� ������ ������ PB7, �� ��������� �������� "�������� �������";
    // �����, ���������� ���� btnOn
    if (!(PIND & (1 << PIND0)))
    {
        OCR1BL++;
        OCR0++;
    }
    else if (!(PIND & (1 << PIND1)))
    {
        OCR1BL--;
        OCR0--;
    }
    else if (!(PINB & (1 << PINB5)))
    {
        if (!flag.btnOn)
        {
            flag.btnOn = 1;
            flag.ledDir++;
        }
    }
    else if (!(PINB & (1 << PINB6)))
    {
        timerOcr--;
    }
    else if (!(PINB & (1 << PINB7)))
    {
        timerOcr++;
    }
    else
    {
        flag.btnOn = 0;
    }
}

/*******************************************************************************
Timer0 Overflow Handler
������������ ��� ���������� ������������ ������� � ����������� ������������
���������� ��� �� �������������� �����������
*******************************************************************************/
uint8_t porta = {0};
ISR(TIMER0_OVF_vect)
{
    // ��������� ������������ �������
    PORTA = porta;
    TimerInc();
    

    // ���������� ������������ ����������
    uint8_t kd = OCR0 * 100 / 256;
    uint8_t buf[4];  // ����� ��� �������� 4-� ���������� ����� ���
                     // ������ �� �������������� ����������

    // �������������� ������������ ���������� � 4-� ��������� �������-����������
    // ����� (����������� ������ ��� ����� ������ ������ ��������� 4)
    SemsegBin2Bcd(kd, buf, sizeof(buf));

    // ����������� �� 4-� �������������� ����������� 4-� ���������� �����
    // (����������� ������ ��� ����� ������ ������ ��������� 4)
    SemsegDisp(buf, sizeof(buf));
}


ISR(TIMER0_COMP_vect)
{    
    SemsegOff();
    porta = PORTA;
    PORTA = 0;
}

/*******************************************************************************
Timer Compare Handler
������������ ��� ������������ ������ ��������� "�������� �������"
*******************************************************************************/
void TimerComp(void)
{
    // ���� ����������� ���� ledDir, �� ���������� �������� ���������
    // "�������� �������" �����;
    // �����, �������� ��������� "�������� �������" ������    
    uint8_t temp = PORTA;
    if (flag.ledDir)
    {
        temp >>= 1;
        temp = temp ? temp : 0x80;
    }
    else
    {
        temp <<= 1;
        temp = temp ? temp : 0x01;
    }
    PORTA = temp;
}
// End File --------------------------------------------------------------------