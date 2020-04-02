#ifndef __SEMSEGSPI_H
#define __SEMSEGSPI_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>        // ������������ ���� ��� ������ � ��� ��
#include <avr/pgmspace.h>  // ����������� ���� ��� ������ � ������� �������� ��

#include "spi.h"  // ������������ ���� ��� ������ � ������� SPI ��

// Function prototypes ---------------------------------------------------------
void SemSegInit(void);
void SemSegDisp(uint8_t* pbufDisp, uint8_t bufDispSize);
void SemsegBin2Bcd(uint16_t data, uint8_t* pbuf, uint8_t bufSize);

#endif
// End File --------------------------------------------------------------------