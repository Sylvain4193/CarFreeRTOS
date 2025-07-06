#ifndef __DELAY_H_
#define __DELAY_H_

#include "main.h"

void Delay_Init(void);
void delay_us(uint32_t nus); // Timing us
void Delay_Ms(uint16_t nms);

#endif
