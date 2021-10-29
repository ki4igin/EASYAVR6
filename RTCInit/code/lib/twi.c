// Includes --------------------------------------------------------------------
#include "twi.h"

// Typedef ---------------------------------------------------------------------
// ��� ��������� �� ���� i2c
typedef enum
{
    LINE_START        = 0x08,  // ������������ ��������� �����
    LINE_REP_START    = 0x10,  // ������������ ��������� ��������
    LINE_TX_SLA_ACK   = 0x18,  // ������� ����� SLA+W � ������� ������������� (ACK)
    LINE_TX_SLA_NACK  = 0x20,  // ������� ����� SLA+W, � ������������� �� ������� (NACK)
    LINE_TX_DATA_ACK  = 0x28,  // ������� ����� ������ � ������� ������������� (ACK)
    LINE_TX_DATA_NACK = 0x30,  // ������� ����� ������, � ������������� �� ������� (NACK)
    LINE_ARB_LOST     = 0x38,  // ������ ���������� ��� �������� ������ ������ ��� ������
    LINE_RX_SLA_ACK   = 0x40,  // ������� ����� SLA+R � ������� ������������� (ACK)
    LINE_RX_SLA_NACK  = 0x48,  // ������� ����� SLA+R � ������� ��������������� (NACK)
    LINE_RX_DATA_ACK  = 0x50,  // ������ ���� ������ � �������� ������������� (ACK)
    LINE_RX_DATA_NACK = 0x58,  // ������ ���� ������ � �������� ��������������� (NACK)
    LINE_BUS_ERROR    = 0x00,  // ������ �� ���� � ���������� ������������� ������������
                               // ��������� ����� ��� ����
} StatusLine_t;

// ��� ��������� ������ TWI
typedef enum
{
    TWI_NOINIT = 0,  // TWI �� ���������������
    TWI_READY,       // TWI ����� � ������/������
    TWI_RUN_TX,      // TWI ����� �������
    TWI_RUN_RX,      // TWI ����� �������
} TwiStatus_t;

// Macro -----------------------------------------------------------------------
#define LINE_FREQ 100000UL  // ������� ������ � ��

// Variables -------------------------------------------------------------------
static uint8_t twiSlaW    = {0};  // ����� �������� ����������: SLA+W
static uint8_t twiRegAddr = {0};  // ����� ���������� �������� ��� ������/������

static TwiStatus_t twiStatus = TWI_NOINIT;  // ��������� ������ TWI

static uint8_t* pbufTwi;     // ��������� �� ����� ��� ������/������
static uint8_t  bufSizeTwi;  // ������ ������ ��� ������/������

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������������� ������ TWI
�� ��������� ������������� ��������������� ������ TWI_READY
������� ������ � ��: LINE_FREQ
������������ ���������� �� �������� �� ���� i2c
*******************************************************************************/
void TwiInit(void)
{
    // ��������� ������������ ������� ��� ���������� ��������� ������� �
    // ���������� ������ ������ TWI � ���������� �� �������� �� ����
    TWBR = (F_CPU / LINE_FREQ - 16) / 2;
    TWCR |= (1 << TWEN) | (1 << TWIE);

    // ��������� ������� TWI_READY
    twiStatus = TWI_READY;
}

/*******************************************************************************
������� ������ ������ � ������� ����������
�� ����� ������ ��������������� ������ TWI_RUN_TX, ����� ���������� ������ 
��������������� ������ TWI_READY

���������:
slaw        ����� �������� ����������: SLA+W
regAddr     ����� ���������� �������� ��� ������
pbuf        ��������� �� ����� ��� ������
bufSize     ������ ������ ��� ������
*******************************************************************************/
void TwiWrite(uint8_t slaw, uint8_t regAddr, uint8_t* pbuf, uint8_t bufSize)
{
    // ���� ������ ������ �� ����� TWI_READY, �� ������� �� �������
    if (twiStatus != TWI_READY)
    {
        return;
    }
    // ��������� ������� TWI_RUN_TX
    twiStatus = TWI_RUN_TX;

    // ��������� ������ �������� ����������, ������ ���������� �������� ���
    // ������, ��������� � ������� ������  ��� ������
    twiSlaW    = slaw;
    twiRegAddr = regAddr;
    pbufTwi    = pbuf;
    bufSizeTwi = bufSize;

    // ������������ ��������� ����� �� ���� TWI, ������ ��������� ��������
    // ��� ������ ������
    TWCR |= (1 << TWSTA);
}

/*******************************************************************************
������� ������ ������ �� ������� ����������
�� ����� ������ ��������������� ������ TWI_RUN_RX, ����� ���������� ������ 
��������������� ������ TWI_READY

���������:
slaw        ����� �������� ����������: SLA+W
regAddr     ����� ���������� �������� ��� ������
pbuf        ��������� �� ����� ��� ������
bufSize     ������ ������ ��� ������
*******************************************************************************/
void TwiRead(uint8_t slaw, uint8_t regAddr, uint8_t* pbuf, uint8_t bufSize)
{
    // ���� ������ ������ �� ����� TWI_READY, �� ������� �� �������
    if (twiStatus != TWI_READY)
    {
        return;
    }
    // ��������� ������� TWI_RUN_RX
    twiStatus = TWI_RUN_RX;

    // ��������� ������ �������� ����������, ������ ���������� �������� ���
    // ������, ��������� � ������� ������ ��� ������
    twiSlaW    = slaw;
    twiRegAddr = regAddr;
    pbufTwi    = pbuf;
    bufSizeTwi = bufSize;

    // ������������ ��������� ����� �� ���� TWI, ������ ��������� ��������
    // ��� ������ ������
    TWCR |= (1 << TWSTA);
}

/*******************************************************************************
������� ��������� ������� �� ���� i2c
��� ������ ������, a������ ����������� ������� ��������� ���� i2c � ���������
����������� �������� ��� �������� � ����� ��������� ��������� �������� ������ TWI.

������������ ��������:
flag        ���������� � ������� ���������� ��������
flag.rxc    ���� ���������� ������
flag.txc    ���� ���������� ������
*******************************************************************************/
TwiFlags_t TwiTwsi(void)
{
    TwiFlags_t  flag   = {0};        // ���������� � ������� ���������� ��������
    TwiStatus_t status = twiStatus;  // ��������� ������ TWI

    // ���������� �������� ��������� ���� i2c � � ����������� �� ����
    // ���������� ����������� �������� ��� �������� � ����� ���������
    StatusLine_t statusLine = TWSR & 0xF8;
    switch (statusLine)
    {
        case LINE_BUS_ERROR:
            // ������������ ��������� ����
            TWCR |= (1 << TWSTO);
            break;
        case LINE_START:
            // �������� ������ SLA+W � ����� ����� TWSTA
            TWDR = twiSlaW;
            TWCR &= ~(1 << TWSTA);
            break;
        case LINE_REP_START:
            // �������� ������ SLA+R � ����� ����� TWSTA
            TWDR = twiSlaW + 1;
            TWCR &= ~(1 << TWSTA);
            break;
        case LINE_TX_SLA_ACK:
            // �������� ����� ������ � ������� �������� �������� ����������
            // ��/� �������� ����� ���������/���������� ������
            TWDR = twiRegAddr;
            break;
        case LINE_TX_SLA_NACK:
            // ���������� ��������: ������������ ��������� ���� � ���������
            // ��������� TWI_READY
            TWCR |= (1 << TWSTO);
            status = TWI_READY;
            break;
        case LINE_TX_DATA_ACK:
            // � ����������� �� ��������� ������ TWI, ��������� � ����������
            // ������ ��� ������ ������ � ������� ����������
            switch (status)
            {
                case TWI_RUN_RX:
                    // ������������ ��������� �������� ��� ���������� ������
                    TWCR |= (1 << TWSTA);
                    break;
                case TWI_RUN_TX:
                    // �������� ����� ������ � ������� ���������� �� ������,
                    // ��������� ��������� � ���������� ������� ������ ��������.
                    // ���� ������ ������ ����� ����, �� ��������� ��������:
                    // ��������� ��������� ����, ������������� ���������
                    // TWI_READY � ���� txc
                    if (!bufSizeTwi)
                    {
                        TWCR |= (1 << TWSTO);
                        status   = TWI_READY;
                        flag.txc = 1;
                    }
                    else
                    {
                        TWDR = *pbufTwi++;
                        bufSizeTwi--;
                    }
                    break;
                default:
                    break;
            }
            break;
        case LINE_TX_DATA_NACK:
            // ���������� ��������: ������������ ��������� ����, ���������
            // ��������� TWI_READY
            TWCR |= (1 << TWSTO);
            status = TWI_READY;
            break;
        case LINE_ARB_LOST:
            break;
        case LINE_RX_SLA_ACK:
            // ���������� ������� ������ ������
            // ���� ��������� ������ ���� ����, �� ����� ��������
            // ��������������� (NACK); ����� �������� ������������� (ACK)
            bufSizeTwi--;
            if (bufSizeTwi)
            {
                TWCR |= (1 << TWEA);
            }
            else
            {
                TWCR &= ~(1 << TWEA);
            }
            break;
        case LINE_RX_SLA_NACK:
            // ���������� ������: ������������ ��������� ����, ���������
            // ��������� TWI_READY
            TWCR |= (1 << TWSTO);
            status = TWI_READY;
            break;
        case LINE_RX_DATA_ACK:
            // ������ ��������� ����� ������ � ����� ������ ��������� ���������,
            // � ���������� ������� ������ ������
            // ���� ������� ��������� ���� ��� ������, �� ��������
            // ��������������� (NACK); ����� �������� �������������(ACK)
            *pbufTwi++ = TWDR;
            bufSizeTwi--;
            if (bufSizeTwi)
            {
                TWCR |= (1 << TWEA);
            }
            else
            {
                TWCR &= ~(1 << TWEA);
            }
            break;
        case LINE_RX_DATA_NACK:
            // ������ ���������� ��������� ����� ������ � ����� ������ �
            // ���������� ������: ������������ ��������� ����, ���������
            // ��������� TWI_READY � ����� txc
            *pbufTwi++ = TWDR;
            TWCR |= (1 << TWSTO);
            status   = TWI_READY;
            flag.rxc = 1;
            break;
        default:
            // ������������ ��������� ���� � ��������� ��������� TWI_READY
            TWCR |= (1 << TWSTO);
            status = TWI_READY;
            break;
    }
    // ����� ����� TWINT, ���������� ��������� ������ TWI
    TWCR |= (1 << TWINT);
    twiStatus = status;
    // ����������� ���������� � ������� ���������� ��������
    return flag;
}
// End File --------------------------------------------------------------------