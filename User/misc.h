#ifndef _MISC_H
#define _MISC_H

#include "includes.h"

void delay_us(u32 us);
void DecToBCD(u16 Dec, u8 *pBCD, u8 len);
float CalculateTemperature(float dat,const u16 Rp,const u16 Bx);
INT16U crc16(INT8U *dat, INT8U len);

#endif