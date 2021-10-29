// Includes --------------------------------------------------------------------
#include "spi.h"
#include <avr/interrupt.h>  // ������������ ���� ��� ������ � ������������ ��
// Typedef ---------------------------------------------------------------------
// ��� ������� ������ SPI
typedef enum
{
    SPI_NOINIT = 0,  // SPI �� ���������������
    SPI_READY,       // SPI ����� � ������
    SPI_BUSY         // SPI �����
} SpiStatus_t;

// Macro -----------------------------------------------------------------------
#define SPI_PORT PORTB  // ���� �� ������� ������� ������ SPI
#define SPI_DDR  DDRB   //
#define SPI_PIN  PINB   //

#define SPI_SCK  7  // ����� ������ SCK ������ SPI0
#define SPI_MISO 6  // ����� ������ MISO ������ SPI
#define SPI_MOSI 5  // ����� ������ MOSI ������ SPI
#define SPI_SS   4  // ����� ������ SS ������ SPI

#define SPI_CS 1  // ����� ������ �� � �������� ��������� ����� CS \
                  // ����������� ������

// Variables -------------------------------------------------------------------
volatile static SpiStatus_t spiStatus = SPI_NOINIT;  // ���������� ������� ������ SPI
static uint8_t*             pbufSpi;                 // ��������� �� ����� ��� ������
static uint8_t              bufSizeSpi;              // ������ ������ ��� ������

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������������� SPI
�� ��������� ������������� ��������������� ������ SPI_READY
�������� ������: F_CPU / 4
�����: Master; CPOL = 0; CPHA = 0
���������� ���������� �� ���������� ������
*******************************************************************************/
void SpiInit(void)
{
    // ������������� �������:
    // SCK, MOSI, SS � CS - ������ (SS ��������������� ��� ����� �����
    // ������ SPI �������� �� ������� � ����� Slave);
    // MISO - ����
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_CS);
    SPI_DDR &= ~(1 << SPI_MISO);

    // ��������� ������ �S � ���. 1
    SPI_PORT |= (1 << SPI_CS);
    SPI_PORT |= (1 << SPI_SS);

    // ��������� ������, ��������� ������ SPI � ���������� �� ���������� ������
    // SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL) | 0x3;
    SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL);

    // ������������ ������ �S ��� ���������� ������������
    SPI_PORT &= ~(1 << SPI_CS);
    SPI_PORT |= (1 << SPI_CS);

    // ������������ ������ �S ��� ���������� ������������
    SPI_PORT &= ~(1 << SPI_SS);
    SPI_PORT |= (1 << SPI_SS);

    // ��������� ������� SPI_READY
    spiStatus = SPI_READY;

    sei();

    while (1)
    {
        uint8_t buf[2] = {0};
        buf[0]         = 0x80 | 0x0F;

        SpiTxRx(buf, sizeof(buf));

        while (spiStatus != SPI_READY)
        {
            ;
        }
        PORTC = buf[1];

        for (uint16_t i = 0; i < UINT16_MAX; i++)
        {
            asm("nop");
        }
        
    }
}

/*******************************************************************************
������� ������ ������� � ������� �����������
�� ����� ������ ��������������� ������ SPI_BUSY, ����� ������ ���������������
������ SPI_READY

���������:
pbuf        ��������� �� ����� ��� ������
bufSize     ������ ������ ��� ������
*******************************************************************************/
void SpiTxRx(uint8_t* pbuf, uint8_t bufSize)
{
    // ���� ������ ������ �� ����� SPI_READY, �� ������� �� �������
    if (spiStatus != SPI_READY)
    {
        return;
    }

    // ��������� ������� SPI_BUSY
    spiStatus = SPI_BUSY;

    // ����� ������ �S (����� �������� ����������)
    SPI_PORT &= ~(1 << SPI_SS);

    // ��������� ������� � ��������� �� ����� ������ �
    // �������� ������� ����� ������ �� ������ ������
    bufSizeSpi = bufSize;
    pbufSpi    = pbuf;
    SPDR       = *pbufSpi;
}

/*******************************************************************************
������� ��������� ������� �� ���������� ������ ����� ������ ������ SPI
�������� ������ ���������� � ����� ������ ������ ���������� ������
*******************************************************************************/
void SpiTc(void)
{
    // ������ ���������� ���� ������ �� SPDR � ��������� ��� � ����� ������ ��
    // ����� ����������� �����
    *pbufSpi = SPDR;

    // ���������� ���������� ���� ������
    // ���� ���������� ���������� ���� ������ �� ����� ����, �� ��������
    // ��������� ���� ������ �� ������ ������;
    // �����, ��������� �����: ������������� ����� CS � ������ SPI_READY
    bufSizeSpi--;
    if (bufSizeSpi)
    {
        SPDR = *++pbufSpi;
    }
    else
    {
        SPI_PORT |= (1 << SPI_SS);
        spiStatus = SPI_READY;
    };
}
// End File --------------------------------------------------------------------