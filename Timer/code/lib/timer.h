#ifndef __TIMER_H
#define __TIMER_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>

// Function prototypes ---------------------------------------------------------
void TimerInit(uint8_t ocr);
void TimerInc(void);
void TimerComp(void);
// Inline Functions ------------------------------------------------------------

#endif
// End File --------------------------------------------------------------------