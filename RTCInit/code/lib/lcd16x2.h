#ifndef __LCD16X2_H
#define __LCD16X2_H

// Includes --------------------------------------------------------------------
#include <avr/io.h>      // ������������ ���� ��� ������ � ��� ��
#include <util/delay.h>  // ������������ ���� ��� ������ � ����������

// Macro -----------------------------------------------------------------------
// ������� ���� ��� ������ LCD �������

// ������� ������� � ���������� ������� � ������ ������ ������
#define LCD_CLR 0  // ������� ��� ���������� (DB0)

// ��������� ������� � ������ ������ ������
#define LCD_HOME 1  // ������� ��� ���������� (DB1)

// ��������� ����������� ������ ��������, ���������� ������ ������
#define LCD_MODE     2  // ������� ��� ���������� (DB2)
#define LCD_MODE_I_D 1  // ����������� ������ �������� (DB1)
                        // 0: ������ ������; 1: ����� �������
#define LCD_MODE_SH 0   // ���������� ������ ������ ��� ������ �������� (DB0)

// ���������/���������� ������� � ����������� �������
#define LCD_ON   3  // ������� ��� ���������� (DB3)
#define LCD_ON_D 2  // ��������� ������� (DB2)
#define LCD_ON_C 1  // ��������� ������� (DB1)
#define LCD_ON_B 0  // ��������� ������� ������� (DB0)

// ����� ������� � ������
#define LCD_SHIFT     4  // ������� ��� ���������� (DB4)
#define LCD_SHIFT_S_C 3  // ���������� ������ ������ (DB3)
                         // 0: ����� ������ �������; 1: ����� ������� � ������
#define LCD_SHIFT_R_L 2  // ����������� ������ (DB2)
                         // 0: ����� �����; 1: ����� ������

// ��������� ����������, ���������� ����� ��� ������, ������� ������
#define LCD_F    5  // ������� ��� ���������� (DB5)
#define LCD_F_DL 4  // ����� ���������� (DB4)
                    // 0: 4-������ ���������; 1: 8-������ ���������
#define LCD_F_N 3   // ���������� ����� ��� ������ (DB3)
                    // 0: 1 ������; 1: 2 ������
#define LCD_F_F 2   // ������ ������ (DB2) 0: 5�8 ��������; 1: 5�11 ��������

// ������ ������ DDRAM ������ � �������� ���������
#define LCD_DDRAM 7  // ������� ��� ���������� (DB7)

// Function prototypes ---------------------------------------------------------
void LcdInit(void);
void LcdDispBuf(uint8_t* pbuf, uint8_t bufSize);
void LcdDispStr(uint8_t* str);
void LcdMovCursor(uint8_t newPos);
void LcdSendCmd(uint8_t cmd);
void LcdSendChar(uint8_t data);

#endif
// End File --------------------------------------------------------------------