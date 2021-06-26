// Includes --------------------------------------------------------------------
#include "onewire.h"

// Typedef ---------------------------------------------------------------------

// Macro -----------------------------------------------------------------------
#define ONEWIRE_PORT     PORTA       //
#define ONEWIRE_DDR      DDRA        //
#define ONEWIRE_PIN      PINA        //
#define ONEWIRE_PIN_MASK (1 << PA4)  //

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
static inline void OnewireHigh(void)
{
    ONEWIRE_DDR &= ~ONEWIRE_PIN_MASK;
}

static inline void OnewireLow(void)
{
    ONEWIRE_DDR |= ONEWIRE_PIN_MASK;
}

static inline uint8_t OnewireLevel(void)
{
    return ONEWIRE_PIN & ONEWIRE_PIN_MASK;
}

// Functions -------------------------------------------------------------------
void OnewireInit()
{
    ONEWIRE_PORT &= ~ONEWIRE_PIN_MASK;
    OnewireHigh();
}

// ����� ������� ������ (reset), ������� �������� ������� �����������.
// ���� ������� ����������� �������, ���������� ��� ���������� � ���������� 1, ����� ���������� 0
uint8_t OnewireReset()
{
    OnewireLow();
    _delay_us(640);  // ����� 480..960 ���
    OnewireHigh();
    _delay_us(2);  // ����� ����������� �������������� ���������, ����� ������� ������� ������� �� ����
    // ��� �� ����� 60 �� �� ��������� �������� �����������;
    for (uint8_t i = 80; i; i--)
    {
        if (OnewireLevel() == 0)
        {
            // ���� ��������� ������� �����������, ��� ��� ���������
            while (OnewireLevel() == 0)
            {
            }  // ��� ����� ������� �����������
            return 1;
        }
        _delay_us(1);
    }
    return 0;
}

// ���������� ���� ���
// bit ������������ ��������, 0 - ����, ����� ������ �������� - �������
void OnewireSendBit(uint8_t bit)
{
    OnewireLow();
    if (bit)
    {
        _delay_us(5);  // ������ �������, �� 1 �� 15 ��� (� ������ ������� �������������� ������)
        OnewireHigh();
        _delay_us(90);  // �������� �� ���������� ��������� (�� ����� 60 ���)
    }
    else
    {
        _delay_us(90);  // ������ ������� �� ���� �������� (�� ����� 60 ���, �� ����� 120 ���)
        OnewireHigh();
        _delay_us(5);  // ����� �������������� �������� ������� �� ���� + 1 �� (�������)
    }
}

// ���������� ���� ����, ������ ������ ���, ������� ��� �����
// data - ������������ ��������
void OnewireSend(uint8_t data)
{
    for (uint8_t i = 8; i; i--)
    {
        OnewireSendBit(data & 1);
        data >>= 1;
    }
}

// ������ �������� ����, ������������ ������������.
// ���������� 0 - ���� ������� 0, �������� �� ���� �������� - ���� �������� �������
uint8_t OnewireReadBit()
{
    OnewireLow();
    _delay_us(2);  // ������������ ������� ������, ������� 1 ���
    OnewireHigh();
    _delay_us(8);  // ����� �� ������� �������������, ����� �� ����� 15 ���
    uint8_t bit = OnewireLevel();
    _delay_us(80);  // �������� �� ���������� ����-�����, ������� 60 ��� � ������ ������� ������
    return bit;
}

// ������ ���� ����, ���������� �����������, ������� ��� �����, ���������� ����������� ��������
uint8_t OnewireRead()
{
    uint8_t data = 0;
    for (uint8_t i = 8; i; i--)
    {
        data >>= 1;
        if (OnewireReadBit())
        {
            data |= 0x80;
        }
    }
    return data;
}

// End File --------------------------------------------------------------------