#ifndef __PCF8583_H
#define __PCF8583_H

// Includes --------------------------------------------------------------------
#include "twi.h"  // ������������ ���� ��� ������ � ������� TWI ��

// Typedef ---------------------------------------------------------------------
// ��� �������
typedef struct
{
    uint8_t seconds;  // �������
    uint8_t minutes;  // ������
} Time_t;

// Function prototypes ---------------------------------------------------------
void PcfReadTime(Time_t* time);

#endif
// End File --------------------------------------------------------------------