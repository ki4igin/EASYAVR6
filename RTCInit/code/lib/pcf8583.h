#ifndef __PCF8583_H
#define __PCF8583_H

// Includes --------------------------------------------------------------------
#include "twi.h"  // Заголовочный файл для работы с модулем TWI МК

// Typedef ---------------------------------------------------------------------
// Тип времени
typedef struct
{
    uint8_t seconds;       // Секунды
    uint8_t minutes;       // Минуты
    uint8_t hours;         // Часы
    uint8_t days : 6;      // Дни
    uint8_t years : 2;     // Год
    uint8_t months : 5;    // Месяц
    uint8_t weekdays : 3;  // День недели
} Time_t;

// Function prototypes ---------------------------------------------------------
void PcfInit(void);
void PcfReadTime(Time_t* time);

#endif
// End File --------------------------------------------------------------------