// Includes --------------------------------------------------------------------
#include "semsegspi.h"

// Macro -----------------------------------------------------------------------
/*******************************************************************************
�������� ����� ����������� ������ MCP23S17 
*******************************************************************************/
#define SLAW 0x40  // �������� ���� ��� ������ � ����������� ������
#define SLAR 0x41  // �������� ���� ��� ������ �� ����������� ������

/*******************************************************************************
�������� ����������� ������ MCP23S17
*******************************************************************************/
// IODIRA, IODIRB - �������� ����������� ������� ������ A, B
#define IODIRA 0x00  // �������� ����� ������ - 0b1111_1111
#define IODIRB 0x01  // �������� ����� ������ - 0b1111_1111
// ������� ���� ��������� IODIRA, IODIRB
#define IO7 7  //
#define IO6 6  // IO� 1: ����� x �������� �� ����
#define IO5 5  // IO� 0: ����� x �������� �� �����
#define IO4 4  //
#define IO3 3  //
#define IO2 2  //
#define IO1 1  //
#define IO0 0  //

// IPOLA, IPOLB - �������� ��������� ���������� ������ A, B
// (��� �������, ����������� �� ����)
#define IPOLA 0x02  // �������� ����� ������ - 0b0000_0000
#define IPOLB 0x03  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� IPOLA, IPOLB
#define IP7 7  //
#define IP6 6  // IPx 1: ����� � - ���� � ���������
#define IP5 5  // IPx 0: ����� � - ���� ��� ��������
#define IP4 4  //
#define IP3 3  //
#define IP2 2  //
#define IP1 1  //
#define IP0 0  //

// GPINTENA, GPINTENB - �������� ���/���� ������� ���������� ������ A, B
#define GPINTENA 0x04  // �������� ����� ������ - 0b0000_0000
#define GPINTENB 0x05  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� GPINTENA, GPINTENB
#define GPINT7 7  //
#define GPINT6 6  // GPINTx 1: ��������� �������� ���������� �� ������ �
#define GPINT5 5  // GPINTx 0: ���������� �������� ���������� �� ������ �
#define GPINT4 4  //
#define GPINT3 3  //
#define GPINT2 2  //
#define GPINT1 1  //
#define GPINT0 0  //

// DEFVALA, DEFVALB - �������� ��������� ������� ���������� ������ A, B
#define DEFVALA 0x06  // �������� ����� ������ - 0b0000_0000
#define DEFVALB 0x07  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� DEFVALA, DEFVALB
#define DEF7 7  //
#define DEF6 6  // ���� �� ����� ������ � ���������� ������� �� �����
#define DEF5 5  // �������� DEF�, �� ������������ ����������.
#define DEF4 4  //
#define DEF3 3  //
#define DEF2 2  //
#define DEF1 1  //
#define DEF0 0  //

// INTCONA, INTCONB - �������� ���������� �������� ������������ ������ A, B
#define INTCONA 0x08  // �������� ����� ������ - 0b0000_0000
#define INTCONB 0x09  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� INTCONA, INTCONB
#define IOC7 7  //
#define IOC6 6  // IOC� 1:  ���������� ������������ ���� ���������� �������
#define IOC5 5  //          ����� � �� ����� �������� ���� DEF� �������� DEFVAL�.
#define IOC4 4  // IOC� 0:  ���������� ������������ ��� ��������� �����������
#define IOC3 3  //          ������ �� ����� �.
#define IOC2 2  //
#define IOC1 1  //
#define IOC0 0  //

// IOCON - ������� ������������ ����������� �����
// IOCON ���������� �� ���� ������� 0x0A � 0x0B, ���������� ��������� ���
// ���������������� ������/������ ���������� ���������.
#define IOCON 0x0A  // �������� ����� ������ - 0b0000_0000
// #define IOCON  0x0B // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� IOCON
#define BANK   7  // 1 �������� ��������� �� ������ ����� ������� 
                  // 0 �������� ��������� � ����� ����� ������ (������ ����������������)
#define MIRROR 6  // 1 ���������� � ����� ����� �/� �������� ������������ INTA � INTB 
                  // 0 ���������� � ����� � �������� ������ INTA, � ����� � ������ INTB
#define SEQOP  5  // 1 ���������� �������������� ������� ��������� 
                  // 0 ��������� �������������� ������� ���������
#define DISSLW 4  // 1 ���������� �������� �������� ���������� ������ SDA 
                  // 0 ��������� �������� �������� ���������� ������ SDA
#define HAEN   3  // 1 ��������� �������� ��������� MCP23S17 (�2:0) 
                  // 0 ���������� �������� ��������� MCP23S17 (�2:0)
#define ODR    2  // 1 ����� INTA/B - ����� � �������� ����������� 
                  // 0 ����� INTA/B - ������� ����� (INTPOL ������������� ����������)
#define INTPOL 1  // 1 �������� ������� ������ INTA/B ������� 
                  // 0 �������� ������� ������ INTA/B ������

// GPPUA, GPPUB - �������� ��������� PullUp ������� A, B (��� �������, ����������� �� ����)
#define GPPUA 0x0C  // �������� ����� ������ - 0b0000_0000
#define GPPUB 0x0D  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� GPPUA, GPPUB
#define PU7 7  //
#define PU6 6  // PU� 1: ��������� Pull Up ������ x
#define PU5 5  // PU� 0: ���������� Pull Up ������ x
#define PU4 4  //
#define PU3 3  //
#define PU2 2  //
#define PU1 1  //
#define PU0 0  //

// INTFA, INTFB - �������� ������ ���������� ������ A, B (��� ������)
#define INTFA 0x0E  // �������� ����� ������ - 0b0000_0000
#define INTFB 0x0F  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� INTFA, INTFB
#define INF7 7  //
#define INF6 6  // ��� INF� ���������, ��� ���������� ��������� ��
#define INF5 5  // ������ �
#define INF4 4  //
#define INF3 3  //
#define INF2 2  //
#define INF1 1  //
#define INF0 0  //

// INTCAPA, INTCAPB - �������� ������� ��������� ������� ������ A, B
#define INTCAPA 0x10  // �������� ����� ������ - 0b0000_0000
#define INTCAPB 0x11  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� INTCAPA, INTCAPB
#define ICP7 7  //
#define ICP6 6  // ������� INTCAP� ��������� ��������� �������� GPIO�
#define ICP5 5  // ��� ������������� �������� ���������� ����� �.
#define ICP4 4  // INTCAP� ��������� ������� �� INTCAP� ��� GPIO�
#define ICP3 3  //
#define ICP2 2  //
#define ICP1 1  //
#define ICP0 0  //

// GPIOA, GPIOB - �������� �������� ������ A, B
#define GPIOA 0x12  // �������� ����� ������ - 0b0000_0000
#define GPIOB 0x13  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� GPIOA, GPIOB
#define GP7 7  //
#define GP6 6  // GP� 1: �� ����� � ������� ���������� �������
#define GP5 5  // GP� 0: �� ����� � ������� ���������� �������
#define GP4 4  //
#define GP3 3  //
#define GP2 2  //
#define GP1 1  //
#define GP0 0  //

// OLATA, OLATB - �������� ��������� ������ ������� ������ A, B
#define OLATA 0x14  // �������� ����� ������ - 0b0000_0000
#define OLATB 0x15  // �������� ����� ������ - 0b0000_0000
// ������� ���� ��������� OLATA, OLATB
#define OL7 7  //
#define OL6 6  // OL� 1: ���������� �� ������ � ���������� �������
#define OL5 5  // OL� 0: ���������� �� ������ � ���������� ����
#define OL4 4  //
#define OL3 3  //
#define OL2 2  //
#define OL1 1  //
#define OL0 0  //

// Constants -------------------------------------------------------------------
// ������� ������������� �������� � ������ ��������
const uint8_t tableDecoder[17] PROGMEM = {
    0b11111100,   // ��� "0"
    0b01100000,   // ��� "1"
    0b11011010,   // ��� "2"
    0b11110010,   // ��� "3"
    0b01100110,   // ��� "4"
    0b10110110,   // ��� "5"
    0b10111110,   // ��� "6"
    0b11100000,   // ��� "7"
    0b11111110,   // ��� "8"
    0b11110110,   // ��� "9"
    0b11101110,   // ��� "A"
    0b00111110,   // ��� "b"
    0b10011100,   // ��� "C"
    0b01111010,   // ��� "d"
    0b10011110,   // ��� "E"
    0b10001110,   // ��� "F"
    0b00000000};  // ��� ""

// Variables -------------------------------------------------------------------
static uint8_t semSegBuf[4] = {0};  // ������ ������ ��� �������� � ����������� ������

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������������� 4-� �������������� �����������
������ ������ ����������� A:G, DP ���������� � ������� GPB7:GPB0 (P1.7:P1.0)
����������� ������.
������ ���������� ������������ 1:4 ���������� � ������� GP�3:GP�0 (P0.3:P1.0)
����������� ������.
*******************************************************************************/
void SemSegInit(void)
{
    // ������������ ������� ������ ��� �������� � ����������� ������:
    // 1-� ����: �������� ���� ��� ������
    // 2-� ����: ����� ������� �������� ��� ������ (������ � IODIRA � IODIRB)
    // 3-� ����: ��������� ������� GP�3:GP�0 ��� ������� (������� IODIRA)
    // 4-� ����: ��������� ������� GPB7:GPB0 ��� ������� (������� IODIRB)
    uint8_t* pbuf = semSegBuf;

    *pbuf++ = SLAW;
    *pbuf++ = IODIRA;
    *pbuf++ = 0xF0;
    *pbuf++ = 0;

    // ����� ������� � ������������ ������, �������� � ��������� 4 �����
    SpiTxRx(semSegBuf, 4);
}

/*******************************************************************************
������� ����������� 4-� ���������� �������-����������� ����� �� 4-�
�������������� �����������. ���������� ���������� � ����������� ������,
��������� ����� ������������ ������ � �� - SPI. ��� ������ ������������
������������ ���������, ������� ����� ������� ������ ������������� � �����.

���������:
buf:        ��������� �� 4-� ������� �����, � ������� �����������
            4-� ��������� �������-���������� �����.
bufSize:    ������ ������, ������ ������ ��������� 4
*******************************************************************************/
void SemSegDisp(uint8_t* pbuf, uint8_t bufSize)
{
    if (bufSize != 4)
    {
        return;
    }

    static uint8_t ind;  // ����� ���������� (�� 0 �� 3)

    // ������������ ������� ������ ��� �������� � ����������� ������:
    // 1-� ����: �������� ���� ��� ������
    // 2-� ����: ����� ������� �������� ��� ������ (������ � OLATA � OLATB)
    // 3-� ����: ��������� ������� GP�� (������� OLATA) - ��������� ��������� ����������
    // 4-� ����: ��������� ������� GPB� (������� OLATB) - ��������� ���������� �������
    uint8_t* psemSegBuf = semSegBuf;

    *psemSegBuf++ = SLAW;
    *psemSegBuf++ = OLATA;
    *psemSegBuf++ = (1 << ind);
    uint8_t temp  = pbuf[ind];
    *psemSegBuf++ = pgm_read_byte(&tableDecoder[temp]);

    // ���������� ������ ����������
    // ���� ����� ���������� ������ ��� ����� 4, �� ���������� �����
    ind++;
    if (ind >= 4)
    {
        ind = 0;
    }

    // ����� ������� � ������������ ������, �������� � ��������� 4 �����
    SpiTxRx(semSegBuf, 4);
}

/*******************************************************************************
������� �������� 2-� �������� ����� � 4-� ��������� �������-���������� �����.

���������:
data:       �������� 2-������� �����
buf:        ��������� �� 4-� ������� �����, � ������� ������������ ���������������
            4-� ��������� �������-���������� �����.
bufSize:    ������ ������, ������ ������ ��������� 4
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
// End File --------------------------------------------------------------------