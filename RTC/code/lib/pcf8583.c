// Includes --------------------------------------------------------------------
#include "pcf8583.h"

// Macro -----------------------------------------------------------------------
#define PCF_SLAW 0xA0  // �������� ���� ��� ������ ����� ��������� ������� PCF8583

// �������� ����� ��������� ������� PCF8583
#define PCF_CS    0x00  // ������� ���������� � �������
#define PCF_HSEC  0x01  // ������� ����� ������
#define PCF_SEC   0x02  // ������� ������
#define PCF_MIN   0x03  // ������� �����
#define PCF_HOURS 0x04  // ������� �����

// Functions -------------------------------------------------------------------
/*******************************************************************************
������� ������ ������� �� ����� ��������� ������� PCF8583

���������:
ptime       ��������� �� ���������, � ������� ����� �������� �����
*******************************************************************************/
void PcfReadTime(Time_t* ptime)
{
    TwiRead(PCF_SLAW, PCF_SEC, (uint8_t*)ptime, sizeof(*ptime));
}

// End File --------------------------------------------------------------------