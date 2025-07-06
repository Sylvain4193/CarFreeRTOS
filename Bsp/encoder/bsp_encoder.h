/*
 * bsp_encoder.h
 *
 */

#ifndef BSP_ENCODER_H_
#define BSP_ENCODER_H_
#include "bsp.h"

// One full turn of the wheel, the number of pulses picked up by the coder: 30*13*2*2
//#define ENCODER_CIRCLE (1040)

void Encoder_Update_Count(void);
int Encoder_Get_Count_Now(uint8_t Motor_id);
void Encoder_Get_ALL(int *Encoder_all);

#endif /* BSP_ENCODER_H_ */
