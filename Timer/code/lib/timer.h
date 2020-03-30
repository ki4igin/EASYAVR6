#ifndef __TIMER_H
#define __TIMER_H

// Includes --------------------------------------------------------------------
#include "settings.h"
#include <avr/io.h>  // ������������ ���� ��� ������ � ��� ��

// Variables -------------------------------------------------------------------
extern uint8_t timerOcr;  // �������� � ����� ��������� ����������� �������

// Function prototypes ---------------------------------------------------------
void TimerInit(uint8_t ocr);
void TimerInc(void);
void TimerComp(void);

#endif
// End File --------------------------------------------------------------------