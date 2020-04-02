// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>         // ������������ ���� ��� ������ � ��� ��
#include <avr/interrupt.h>  // ������������ ���� ��� ������ � ������������ ��

#include "main.h"
#include "semsegspi.h"  // ������������ ���� ��� ������ ��������������
                        // ������������, ������������� ����� ����������� ������

// Handlers --------------------------------------------------------------------
/*******************************************************************************
Timer2 Overflow Handler
������������ ��� ��������� ������� ������
*******************************************************************************/
ISR(TIMER2_OVF_vect)
{
    /* ���� ������ ������ PA0, �� ����������� �������� ��������;
    �����, ���������� ���� ������� ������
    */
    if (!(PINA & (1 << PINA0)))
    {
        cnt++;
        // ���� �������� �������� ������ 9999, �� ���������� �������
        if (cnt > 9999)
        {
            cnt = 0;
        }
    }
    else
    {
        flag.btnOn = 0;
    }
}

/*******************************************************************************
Timer0 Overflow Handler
������������ ��� ����������� �������� �������� �� ������������� �����������
*******************************************************************************/
ISR(TIMER0_OVF_vect)
{
    uint8_t buf[4];  // ����� ��� �������� 4-� ���������� ����� ���
                     // ������ �� �������������� ����������

    // �������������� �������� �������� � 4-� ��������� �������-����������
    // ����� (����������� ������ ��� ����� ������ ������ ��������� 4)
    SemsegBin2Bcd(cnt, buf, 4);

    // ����������� �� 4-� �������������� ����������� 4-� ���������� �����
    // (����������� ������ ��� ����� ������ ������ ��������� 4)
    SemSegDisp(buf, 4);
}

/*******************************************************************************
SPI Transfer Complete Handler
*******************************************************************************/
ISR(SPI_STC_vect)
{
    // ��������� ������� �� ���������� ������ ����� ������ ������ SPI
    SpiTc();
}
// End File --------------------------------------------------------------------