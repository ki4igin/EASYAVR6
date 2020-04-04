// Includes --------------------------------------------------------------------
#include "lcd16x2.h"

// Macro -----------------------------------------------------------------------
#define LCD_PORT PORTD  // ���� � �������� ��������� LCD �������
#define LCD_DDR  DDRD   // ������ ������ LCD ������ ���� ���������� � �������
#define LCD_PIN  PIND   // ������� ����� ����������������

#define LCD_DATA_MASK 0xF0  // ����� ������� ������ �������

#define LCD_E  3  // ����� ������ E (������ ������ � LCD �������������� �� �����)
#define LCD_RS 2  // ����� ������ RS (1 - ������; 0 - ����������)

#define LCD_PIN_MASK (LCD_DATA_MASK | (1 << LCD_RS) | (1 << LCD_E))

// Function prototypes ---------------------------------------------------------
static void LcdSendHfCmd(uint8_t hfcmd);
static void LcdSendByte(uint8_t byte);
static void LcdSendHfByte(uint8_t hfbyte);

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������������ ������� LCD16x2
*******************************************************************************/
void LcdInit(void)
{
    // ������������� �������, ������������ � ������� ��� �������
    LCD_DDR |= LCD_PIN_MASK;

    // �������� �� ����� 15 ��, ����� ������ ������� �� �������
    _delay_ms(15);

    // ������������ ������ � ������� ��� �� 4-� ������ ���������
    // �������� ������� ������������ � datasheet
    // ���� �� �������� �� 4-������ ��������� ���������� ������� �� ��������
    
    // �������� �������� ��������� ������� ��������� 8-������� ����������
    LcdSendHfCmd((1 << LCD_F) | (1 << LCD_F_DL));
    // �������� �� ����� 4.1 ��
    _delay_ms(5);
    // �������� �������� ��������� ������� ��������� 8-������� ����������
    LcdSendHfCmd((1 << LCD_F) | (1 << LCD_F_DL));
    // �������� �� ����� 100 ���
    _delay_us(100);
    // �������� �������� ��������� ������� ��������� 8-������� ����������
    LcdSendHfCmd((1 << LCD_F) | (1 << LCD_F_DL));
    // �������� �� ����� 50 ���
    _delay_us(50);
    // �������� �������� ��������� ������� ��������� 4-������� ����������
    LcdSendHfCmd(1 << LCD_F);

    // ������� ������� �� 4-������ ��������� ��������. ������ ����� ��������
    // �������/������ �� ������� �����    

    // ������������� 4-������� ����������
    _delay_us(50);
    LcdSendCmd(1 << LCD_F);

    // ������� �������, ������� ��������� �� ����� 1.5 ��
    _delay_us(50);
    LcdSendCmd(1 << LCD_CLR);
    _delay_ms(2);

    // ��������� ������ �������
    _delay_us(50);
    LcdSendCmd((1 << LCD_ON) | (1 << LCD_ON_D));
}

/******************************************************************************
������� ������ �� ������� ������� ��������

���������:
pbuf:       ��������� �� ������
bufSize:    ������ �������
*******************************************************************************/
void LcdDispBuf(uint8_t* pbuf, uint8_t bufSize)
{
    do
    {
        _delay_us(50);
        LcdSendChar(*pbuf++);
    } while (--bufSize);
}

/******************************************************************************
������� ������ �� ������� ������

���������:
pstr:       ��������� �� ������
*******************************************************************************/
void LcdDispStr(uint8_t* pstr)
{
    do
    {
        _delay_us(50);
        LcdSendChar(*pstr);
    } while (*++pstr);
}

/******************************************************************************
������� ��������� ������� ������� �������
����� ���������� ���������� 50 ���

���������:
newPos:     ����� ������� ������� �������
*******************************************************************************/
void LcdMovCursor(uint8_t newPos)
{
    uint8_t temp = (1 << LCD_DDRAM) | newPos;
    LcdSendCmd(temp);
}

/******************************************************************************
������� ������ �� ������� �������
����� ���������� ���������� 50 ���

���������:
symbol:     ��� �������
*******************************************************************************/
void LcdSendChar(uint8_t symbol)
{
    LCD_PORT |= (1 << LCD_RS);
    LcdSendByte(symbol);
}

/******************************************************************************
������� �������� ������� �� �������
����� ���������� ������ "������� ������� � ���������� ������� � ������ ������
������" � "��������� ������� � ������ ������ ������" ���������� 1.5 ��;
����� ���������� ��������� ������ ���������� 50 ���.

���������:
cmd:        ��� �������
*******************************************************************************/
void LcdSendCmd(uint8_t cmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    LcdSendByte(cmd);
}

/******************************************************************************
������� �������� �������� ������� �� ������� (������������ ��� �������������)
��� �������� ������� ������ ��������� � ������� ��������� ��������� hfcmd

���������:
hfcmd:      ��� �������� ������� (������� ��������)
*******************************************************************************/
static void LcdSendHfCmd(uint8_t hfcmd)
{
    LCD_PORT &= ~(1 << LCD_RS);
    LcdSendHfByte(hfcmd);
}

/******************************************************************************
������� �������� ����� ������/������� �� �������

���������:
byte:       ���� ������/�������
*******************************************************************************/
static void LcdSendByte(uint8_t byte)
{
    // ������� ���������� ������� ��������, ����� �������
    LcdSendHfByte(byte & 0xF0);
    LcdSendHfByte(byte << 4);
}

/******************************************************************************
������� �������� ��������� ������/������� �� �������
��� ��������� ������ ��������� � ������� ��������� ��������� hfbyte

���������:
hfbyte:     ��� ��������� ������/������� (������� ��������)
*******************************************************************************/
static void LcdSendHfByte(uint8_t hfbyte)
{
    // ��������� ����� ��� ���������� ������ � �������
    LCD_PORT |= (1 << LCD_E);

    // ������ � ���� ������ ������� �������� �������� ��������� �������/������ 
    // (������ ������ ������� ���������� � ������� �������� ����� ��)
    
    uint8_t temp = LCD_PORT;
    temp &= ~LCD_DATA_MASK;
    temp |= hfbyte;
    LCD_PORT = temp;

    // ���������� ������/������� � �������
    LCD_PORT &= ~(1 << LCD_E);
}
// End File --------------------------------------------------------------------