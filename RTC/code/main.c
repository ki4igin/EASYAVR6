/*******************************************************************************
Target:  RTC
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

��������� ��������� ������� ����� (������ � �������) �� ���������� �����
��������� ������� PCF8583 � ���������� ��� �� ������� LCD16x2 � ������� "mm:ss".
����� ����� ����������������� � ������ ��������� ������� �������������� ��
���������� i2c � �������� 100 ���.
*******************************************************************************/

// Includes --------------------------------------------------------------------
#include "main.h"

Time_t  time                         = {0};  // ��������� ��� �������� �������
uint8_t bufLcd[3 * sizeof(time) - 1] = {0};  // ����� ��� ������ ������� �� �������,
                                             // ������ ������ "mm:ss"

// Functions -------------------------------------------------------------------
int main(void)
{
    // ������������� ������
    // PA0...PA7 - ����� � PullUp (� ��� ���������� ������)
    PORTA = 0xFF;

    // ������������� � ��������� ������� �2
    // �����: Normal; ������������: 1024; TOP = 0xFF
    // ��2(PD7) �� ���������
    // ����� ������������:
    // t = 1024 * 256 / 8e6 = 32.768 ��; f = 8e6 / 1024 / 256 = 31 ��
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // ������������� �������
    LcdInit();
    // ����� �� ������� ������ "Time:"
    LcdDispStr((uint8_t *)"Time:");

    // ������������� ������ TWI
    TwiInit();

    // ���������� ���������� ����������
    sei();

    // �������� ����
    while (1)
    {
        // ���� ��������� ������������ ������� �2, �� ����������� ����
        // ������������� � �������� ����� � ����� ��������� �������
        if (TIFR & (1 << TOV2))
        {
            TIFR |= (1 << TOV2);

            PcfReadTime(&time);
        }

        // ���� ���������� ���� timeRxc, ����������� ���� timeRxc �
        // ������� ����� �� �������
        if (flag.timeRxc)
        {
            flag.timeRxc = 0;

            // ������������ ������� ��� ����������� �� �������
            // ������� ���/��� ��������� � ������� ���������, ������� - � �������
            uint8_t *pbufLcd = bufLcd;

            *pbufLcd++ = (time.minutes >> 4) + '0';
            *pbufLcd++ = (time.minutes & 0x0F) + '0';
            *pbufLcd++ = ':';
            *pbufLcd++ = (time.seconds >> 4) + '0';
            *pbufLcd++ = (time.seconds & 0x0F) + '0';

            // ��������� ������� ������� �������
            LcdMovCursor(6);
            // ����������� ������� �� �������
            LcdDispBuf(bufLcd, sizeof(bufLcd) / sizeof(bufLcd[0]));
        }
    }
}
// End File --------------------------------------------------------------------