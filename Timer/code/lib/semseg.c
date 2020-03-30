// Includes --------------------------------------------------------------------
#include "semseg.h"

// Macro -----------------------------------------------------------------------
#define DATA_PORT PORTC  // ���� ������ � �������� ����������
#define DATA_DDR  DDRC   // �������������� ����������.

#define CTRL_PORT PORTB  // ����������� ���� � �������� ���������� �����
#define CTRL_DDR  DDRB   // ������/����� �������������� �����������
#define CTRL_PIN3 3      // ����� ������ ������ �/� ���������� ����������
#define CTRL_PIN2 2      // ����� ������ ������ �/� �������� ����������
#define CTRL_PIN1 1      // ����� ������ ������ �/� ������� ����������
#define CTRL_PIN0 0      // ����� ������ ������ �/� ���������� ����������

#define CTRL_PIN_MASK ((1 << CTRL_PIN3) | (1 << CTRL_PIN2) | \
                       (1 << CTRL_PIN1) | (1 << CTRL_PIN0))

// Constants -------------------------------------------------------------------
// ������� ������������� �������� � ������ ��������
const uint8_t tableDecoder[10] PROGMEM = {
    0b11111100,   // ��� "0"
    0b01100000,   // ��� "1"
    0b11011010,   // ��� "2"
    0b11110010,   // ��� "3"
    0b01100110,   // ��� "4"
    0b10110110,   // ��� "5"
    0b10111110,   // ��� "6"
    0b11100000,   // ��� "7"
    0b11111110,   // ��� "8"
    0b11110110};  // ��� "9"

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������������� 4-� �������������� �����������
������ ������ ���������� � DATA_PORT
������ ���������� ���������� � CTRL_PORT
*******************************************************************************/
void SemsegInit(void)
{
    // ������������� ������� ������������ � �����������
    DATA_DDR  = 0xFF;
    DATA_PORT = 0x00;
    CTRL_DDR |= CTRL_PIN_MASK;
    CTRL_PORT &= ~CTRL_PIN_MASK;
    // ��������� ������� ����������
    CTRL_PORT |= (1 << CTRL_PIN0);
}

/*******************************************************************************
������� �������� 2-� �������� ����� � 4-� ��������� �������-���������� �����.

���������:
data:   �������� 2-������� �����
buf:    ��������� �� 4-� ������� �����, � ������� ������������ ���������������
        4-� ��������� �������-���������� �����.
bufSize: ������ ������, ������ ������ ��������� 4
*******************************************************************************/
void SemsegBin2Bcd(uint16_t data, uint8_t* pbuf, uint8_t bufSize)
{
    if (bufSize != 4)
    {
        return;
    }

    pbuf += bufSize - 1;  // ��������� ��������� ������ �� ��������� (�������)
                          // �������

    *pbuf-- = data / 1000;  // ���������� 4 ������� �������-����������� �����
    data %= 1000;           //
    *pbuf-- = data / 100;   // ���������� 3 ������� �������-����������� �����
    data %= 100;            //
    *pbuf-- = data / 10;    // ���������� 2 ������� �������-����������� �����
    data %= 10;             //
    *pbuf-- = data / 1;     // ���������� 1 ������� �������-����������� �����
    data %= 1;
}

/*******************************************************************************
������� ����������� 4-� ���������� �������-����������� ����� �� 4-�
�������������� �����������.

���������:
buf:    ��������� �� 4-� ������� �����, � ������� �����������
        4-� ��������� �������-���������� �����.
bufSize: ������ ������, ������ ������ ��������� 4
*******************************************************************************/
void SemsegDisp(uint8_t* buf, uint8_t bufSize)
{
    if (bufSize != 4)
    {
        return;
    }

    uint8_t ind = 0;  // ���������� ��� �������� ������� �����

    DATA_PORT = 0x00;  // ���������� ����������� �� �������������� �����������

    if (CTRL_PORT & (1 << CTRL_PIN3))    // ���� ������� 4-� ���������,
    {                                    // �� ��� ���������,
        CTRL_PORT &= ~(1 << CTRL_PIN3);  // �������� 1-� ���������,
        CTRL_PORT |= (1 << CTRL_PIN0);   // � ind ���������� 1-� ������
        ind = buf[0];                    // �������-����������� �����
    }
    else if (CTRL_PORT & (1 << CTRL_PIN0))  // ���� ������� 1-� ���������,
    {                                       // �� ��� ���������,
        CTRL_PORT &= ~(1 << CTRL_PIN0);     // �������� 2-� ���������,
        CTRL_PORT |= (1 << CTRL_PIN1);      // � ind ���������� 2-� ������
        ind = buf[1];                       // �������-����������� �����
    }
    else if (CTRL_PORT & (1 << CTRL_PIN1))  // ���� ������� 2-� ���������,
    {                                       // �� ��� ���������,
        CTRL_PORT &= ~(1 << CTRL_PIN1);     // �������� 3-� ���������,
        CTRL_PORT |= (1 << CTRL_PIN2);      // � ind ���������� 3-� ������
        ind = buf[2];                       // �������-����������� �����
    }
    else if (CTRL_PORT & (1 << CTRL_PIN2))  // ���� ������� 3-� ���������,
    {                                       // �� ��� ���������,
        CTRL_PORT &= ~(1 << CTRL_PIN2);     // �������� 4-� ���������,
        CTRL_PORT |= (1 << CTRL_PIN3);      // � ind ���������� 4-� ������
        ind = buf[3];                       // �������-����������� �����
    }
    else
    {            // ���� �� ���� �� �����������, �� �������,
        return;  //�� ������� �� �������
    }

    // ������ � ���� ������ ��������������� ���������� �������� �������
    // �������-����������� �����, ������� ��������� � ind,
    // ��������� ������� ������������� �������� tableDecoder
    uint8_t temp = pgm_read_byte(&tableDecoder[ind]);
    DATA_PORT    = temp;
}

/*******************************************************************************
������� ���������� �������������� �����������
*******************************************************************************/
void SemsegOff(void)
{
    DATA_PORT = 0x00;
}
// End File --------------------------------------------------------------------