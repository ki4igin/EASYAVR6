// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>         // ������������ ���� ��� ������ � ��� ��
#include <avr/interrupt.h>  // ������������ ���� ��� ������ � ������������ ��

#include "main.h"
#include "lcd16x2.h"  // ������������ ���� ��� ������ � �������� LCD16x2

// Handlers --------------------------------------------------------------------
/*******************************************************************************
Timer2 Overflow Handler
������������ ��� ��������� ������� ������
*******************************************************************************/
ISR(TIMER2_OVF_vect)
{
    // ���� ������ ������ PA5, �� ����������� ��������� ������� ���/����;
    // �����, ���������� ���� ������� ������
    if (!(PINA & (1 << PINA5)))
    {
        // ���� ���� btnOn ����������, �� ������ �� ������, ���������� ���������
        if (!flag.btnOn)
        {
            flag.btnOn = 1;

            // ���� ���� lcdOn ����������, �� ���������� lcdOn � ��������� �������;
            // ����� ������������� lcdOn � �������� �������
            if (flag.lcdOn)
            {
                flag.lcdOn = 0;
                LcdSendCmd(1 << LCD_ON);
            }
            else
            {
                flag.lcdOn = 1;
                LcdSendCmd((1 << LCD_ON) | (1 << LCD_ON_D));
            }
        }
    }
    else
    {
        flag.btnOn = 0;
    }
}

/*******************************************************************************
USART RX Complete Handler
������������ ��� ������ ������ ������
*******************************************************************************/
ISR(USART_RXC_vect)
{
    ;
}
// End File --------------------------------------------------------------------