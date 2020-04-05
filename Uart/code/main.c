/*******************************************************************************
Target:  Uart
Device:  ATmega16;
Board:   EasyAVR6;
Clock:   ext.clock 8 MHz

��������� ������������ ����� �� ���������� RS-232 � ������� ������ USART.
�������� ���������:
    �������� ������ 19200 ���, 
    8 ��� ������,
    1 ����-���,
    ��� �������� � ���.
���������� ��������� ����� ������ ��������� �� 4-� ����:
������ 3 ����� � ��������������; 
��������� � ����������� �����.
������� ������ PB6 ����� ���������� ����������� �������� ����� �� ��������������
�����������, ������� ���������� � ������ B � C. �� ������� ����� ����������
������������ ����� ���������������� �����, �� ���� ������ -  ��� �������� ����.
��� ������� �� ������ PB7 �� �������� ����� ������ ��������� �� 3-� ����:
    1-� ����: 0�80,
    2-� ����: ������� ��������� �� ������� PA0...PA7,
    3-� ����: ����������� �����.
*******************************************************************************/

// Includes --------------------------------------------------------------------
#include "main.h"

// Variables -------------------------------------------------------------------
uint8_t bufTx[NBUF_TX]  = {0};  // ����� ��������
uint8_t bufRx[NBUF_RX]  = {0};  // ����� ������
uint8_t dataRx[NBUF_RX] = {0};  // ������ �������� ������

Flags_t flag = {0};  // ���������� ���������������� ������

// Function prototypes ---------------------------------------------------------
static inline uint8_t Crc(uint8_t* pbuf, uint8_t bufSize);

// Functions -------------------------------------------------------------------
int main(void)
{
    // ������������� ������
    // PB6...PB7 - ����� � PullUp (� ��� ���������� ������)
    // PA0...PA7 - ����� � PullUp (� ��� ��������� ���������� �������������)
    PORTB |= (1 << PB7) | (1 << PB6);
    PORTA = 0xFF;

    // ������������� � ��������� ������� �0
    // �����: Normal; ������������: 64; TOP = 0xFF
    // ���������� ���������� �� ������������
    // ��0(PB3) �� ���������
    // ����� ������������:
    // t = 256 * 64 / 8e6 = 2.048 ��, f = 8e6 / 64 / 256 = 488 ��
    TIMSK |= (1 << TOIE0);
    TCCR0 = (1 << CS01) | (1 << CS00);

    // ������������� ������� �1
    // �����: CTC; ������������: 1; TOP = ��R1A = 0xD800
    // ���������� ���������� �� ����� ���������
    // ��1A(PD5) �� ���������
    // ��1B(PD4) �� ���������
    // ����� ������������ t = ��R1A / 8e6 = 6.9 ��
    OCR1A = 0xD800;
    TIMSK |= (1 << OCIE1A);
    TCCR1B = (1 << WGM12);

    // ������������� � ��������� ������� �2
    // �����: Normal; ������������: 256; TOP = 0xFF
    // ���������� ���������� �� ������������
    // ��2(PD7) �� ���������
    // ����� ������������:
    // t = 256 * 256 / 8e6 = 8.192 ��; f = 8e6 / 256 / 256 = 122 ��
    TIMSK |= (1 << TOIE2);
    TCCR2 = (1 << CS22) | (1 << CS21);

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

    // ������������� �������������� �����������
    SemsegInit();

    // ���������� ���������� ����������
    sei();

    // �������� ����
    while (1)
    {
        // ���� ���� tx ����������, �� ���������� �������� ������
        // ���� ���� rx ����������, �� ������������ �������� ������
        if (flag.tx)
        {
            flag.tx = 0;

            // ������������ ������ ������ ��� ��������
            // 1-� ����: 0�80,
            // 2-� ����: ������� ��������� �� ������� PA0...PA7,
            // 3-� ����: ����������� �����.
            uint8_t* pbuf = bufTx;

            *pbuf++ = 0x80;
            *pbuf++ = PINA;
            *pbuf++ = Crc(bufTx, NBUF_TX - 1);

            // ������ �������� (��������� ���������� �� ����������� ��� UDR)
            UCSRB |= (1 << UDRIE);
        }
        if (flag.rx)
        {
            flag.rx = 0;

            // ���������� ����������� �����
            // ���� ��������� ���� �������� ������ ����� ����������� �����, ��
            // �������� ������ �� ������ ������ bufRx � ������ ������ dataRx
            uint8_t crc = Crc(bufRx, NBUF_RX - 1);

            if (bufRx[NBUF_RX - 1] == crc)
            {
                uint8_t* pbufRx  = bufRx;
                uint8_t* pdataRx = dataRx;
                for (uint8_t i = 0; i < NBUF_RX; i++)
                {
                    *pdataRx++ = *pbufRx++;
                }
            }
        }
    }
}

// Inline Functions ------------------------------------------------------------
/*******************************************************************************
������� ���������� ����������� ����� �������

���������:
pbuf:       ��������� �� ������        
bufSize:    ������ ������

������������ ��������:
����������� �����
*******************************************************************************/
static inline uint8_t Crc(uint8_t* pbuf, uint8_t bufSize)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < bufSize; i++)
    {
        crc += *pbuf++;
    }
    return crc;
}
// End File --------------------------------------------------------------------