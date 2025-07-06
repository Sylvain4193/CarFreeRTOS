#ifndef __BSP_PID_MOTOR_H
#define __BSP_PID_MOTOR_H

#include "bsp.h"

#define PI (3.1415926f)

#define PID_MOTOR_KP (1.5f)
#define PID_MOTOR_KI (0.08f)
#define PID_MOTOR_KD (0.5f)

#define PID_DEF_KP (0.8f)
#define PID_DEF_KI (0.06f)
#define PID_DEF_KD (0.5f)

#define PID_YAW_DEF_KP (0.4)
#define PID_YAW_DEF_KI (0.0)
#define PID_YAW_DEF_KD (0.1)

typedef struct _pid
{
    float target_val;
    float output_val;
    float pwm_output;
    float Kp, Ki, Kd;
    float err;
    float err_last;

    float err_next;
    float integral;
} PID_t;

typedef struct
{
    float SetPoint;   // Desired value
    float Proportion; // Proportional Const
    float Integral;   // Integral Const
    float Derivative; // Derivative Const
    float LastError;  // Error[-1]
    float PrevError;  // Error[-2]
    float SumError;   // Sums of Errors
} PID;

typedef struct _motor_data_t
{
    float speed_mm_s[4];
    float speed_pwm[4];
    int16_t speed_set[4];
} motor_data_t;

void PID_Param_Init(void);

float PID_Location_Calc(PID_t *pid, float actual_val);
void PID_Calc_Motor(motor_data_t *motor);
float PID_Calc_One_Motor(uint8_t motor_id, float now_speed);
void PID_Set_Motor_Target(uint8_t motor_id, float target);
void PID_Clear_Motor(uint8_t motor_id);
void PID_Set_Motor_Parm(uint8_t motor_id, float kp, float ki, float kd);
float PID_Incre_Calc(PID_t *pid, float actual_val);

void PID_Yaw_Reset(float yaw);
float PID_Yaw_Calc(float NextPoint);
void PID_Yaw_Set_Parm(float kp, float ki, float kd);

#endif
