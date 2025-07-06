
#ifndef __BSP_ULTRASONIC_H__
#define __BSP_ULTRASONIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "bsp.h"
extern uint32_t ultrasonic_num;
extern uint8_t ultrasonic_flag;
    float Get_distance(void);
    float Get_distance_default(int nbOccur);

#ifdef __cplusplus
}
#endif
#endif /*__BSP_ULTRASONIC_H__*/
