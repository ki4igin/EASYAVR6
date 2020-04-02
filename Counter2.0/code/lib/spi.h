#ifndef __SPI_H
#define __SPI_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>  // ������������ ���� ��� ������ � ��� ��

// Function prototypes ---------------------------------------------------------
void SpiInit(void);
void SpiTxRx(uint8_t* pbuf, uint8_t bufSize);
void SpiTc(void);

#endif
// End File --------------------------------------------------------------------