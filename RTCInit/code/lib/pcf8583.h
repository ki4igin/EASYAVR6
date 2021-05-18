#ifndef __PCF8583_H
#define __PCF8583_H

// Includes --------------------------------------------------------------------
#include "twi.h"  // ������������ ���� ��� ������ � ������� TWI ��

// Typedef ---------------------------------------------------------------------
// ��� �������
typedef struct
{
    uint8_t seconds;       // �������
    uint8_t minutes;       // ������
    uint8_t hours;         // ����
    uint8_t days : 6;      // ���
    uint8_t years : 2;     // ���
    uint8_t months : 5;    // �����
    uint8_t weekdays : 3;  // ���� ������
} Time_t;

// Function prototypes ---------------------------------------------------------
void PcfInit(void);
void PcfReadTime(Time_t* time);

#endif
// End File --------------------------------------------------------------------