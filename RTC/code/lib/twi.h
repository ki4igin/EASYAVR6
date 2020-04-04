#ifndef __TWI_H
#define __TWI_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>  // Заголовочный файл для работы с РВВ МК

// Typedef ---------------------------------------------------------------------
// Тип флагов завершения операций
typedef struct
{
    uint8_t rxc : 1;  // Флаг завершения чтения
    uint8_t txc : 1;  // Флаг завершения записи
} TwiFlags_t;

// Function prototypes ---------------------------------------------------------
void TwiInit(void);
void TwiWrite(uint8_t sla, uint8_t regAddr, uint8_t* pbuf, uint8_t bufSize);
void TwiRead(uint8_t sla, uint8_t regAddr, uint8_t* pbuf, uint8_t bufSize);

TwiFlags_t TwiTwsi(void);

#endif
// End File --------------------------------------------------------------------