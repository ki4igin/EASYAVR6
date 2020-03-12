#ifndef __SEMSEG_H
#define __SEMSEG_H

// Includes --------------------------------------------------------------------
#include "settings.h"
// Typedef ---------------------------------------------------------------------

// Macro -----------------------------------------------------------------------

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
void SemsegInit(void);
void SemsegBin2Bcd(uint16_t data, uint8_t* buf, uint8_t bufSize);
void SemsegDisp(void);
// Inline Functions ------------------------------------------------------------

#endif
