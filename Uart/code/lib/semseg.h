#ifndef __SEMSEG_H
#define __SEMSEG_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>

// Function prototypes ---------------------------------------------------------
void SemsegInit(void);
void SemsegBin2Bcd(uint16_t data, uint8_t* buf, uint8_t bufSize);
void SemsegDisp(uint8_t* buf, uint8_t bufSize);
void SemsegOff(void);

#endif
// End File --------------------------------------------------------------------