/*******************************************************************************
Target:  Lcd
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

��������� ������� �� ������� LCD16x2 �������������� ��������� "Hello World!"
�� ������� ������ Cancel (PA5) ����� ������� ���������� � �����������
*******************************************************************************/

// Includes --------------------------------------------------------------------
#include "main.h"
#include "onewire.h"

void Num2Str(uint16_t data, uint8_t* pbuf, uint8_t bufSize);
// Functions -------------------------------------------------------------------
int main(void)
{
    // ������������� ������
    // PA5 - ���� � PullUp (� ���� ���������� ������)
    PORTA |= (1 << PA5);

    // ������������� � ��������� ������� �2
    // �����: Normal; ������������: 1024; TOP = 0xFF
    // ���������� ���������� �� ������������
    // ��2(PD7) �� ���������
    // ����� ������������:
    // t = 1024 * 256 / 8e6 = 32.768 ��; f = 8e6 / 1024 / 256 = 31 ��
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // ������������� UART, ��������� ��������� � �����������
    // C������� ������: 19200 ���, 8 ��� ������, 1 ����-���, ��� ��������: ���
    // ���������� ���������� �� ������
    // PD0(RXD) - ���� � PullUp, PD1(TXD) - �����
    PORTD |= (1 << PD0);

    uint16_t ubrr = F_CPU / (16UL * 19200) - 1;

    UBRRH = (uint8_t)(ubrr >> 8);  // ������ �������� ������������
    UBRRL = (uint8_t)ubrr;

    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
    UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);

    // ������������� ������� � ����� ������ "Hello World!"
    LcdInit();
    LcdDispStr((uint8_t*)"Temp = +000.0 C");

    OnewireInit();

    // ���������� ���������� ����������
    sei();
    _delay_ms(1000);

    // �������� ����
    while (1)
    {
        OnewireReset();
        // while (1)
        // {
        //     ;
        // }

        OnewireSend(0xCC);
        OnewireSend(0x44);
        _delay_ms(900);
        OnewireReset();
        OnewireSend(0xCC);
        OnewireSend(0xBE);
        uint8_t buf[9] = {0};
        for (uint8_t i = 0; i < 9; i++)
        {
            buf[i] = OnewireRead();
        }
        for (uint8_t i = 0; i < 9; i++)
        {
            while ((UCSRA & (1 << UDRE)) == 0)
            {
                ;
            }
            UDR = buf[i];
        }

        // uint16_t temp = buf[0] * 5;

        uint16_t temp = (buf[0] & 0xFE) * 5 + (100 * (buf[7] - buf[6]) / buf[7] - 25) / 10;

        uint8_t tempStr[5] = {0};
        Num2Str(temp, tempStr, sizeof(tempStr));

        uint8_t  bufLcd[6] = {0};
        uint8_t* pbufLcd   = bufLcd;

        *pbufLcd++ = '+';
        *pbufLcd++ = tempStr[1];
        *pbufLcd++ = tempStr[2];
        *pbufLcd++ = tempStr[3];
        *pbufLcd++ = '.';
        *pbufLcd++ = tempStr[4];

        // ��������� ������� ������� ������� � ����������� ������� �� �������
        LcdMovCursor(7);
        LcdDispBuf(bufLcd, sizeof(bufLcd) / sizeof(bufLcd[0]));
    }
}

void Num2Str(uint16_t data, uint8_t* pbuf, uint8_t bufSize)
{
    if (bufSize != 5)
    {
        return;
    }

    *pbuf++ = data / 10000 + '0';  // ���������� ������� 5 ������� �����
    data %= 10000;                 //
    *pbuf++ = data / 1000 + '0';   // ���������� ������� 4 ������� �����
    data %= 1000;                  //
    *pbuf++ = data / 100 + '0';    // ���������� ������� 3 ������� �����
    data %= 100;                   //
    *pbuf++ = data / 10 + '0';     // ���������� ������� 2 ������� �����
    data %= 10;                    //
    *pbuf++ = data / 1 + '0';      // ���������� ������� 1 ������� �����
    data %= 1;
}

// End File --------------------------------------------------------------------