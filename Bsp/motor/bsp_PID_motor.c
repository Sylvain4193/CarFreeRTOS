#include "bsp_PID_motor.h"

PID_t pid_motor[4];

//YAW yaw angle
PID pid_Yaw = {0, 0.4, 0, 0.1, 0, 0, 0};

//Initialize PID parameters
void PID_Param_Init(void)
{
	//Speed dependent initialization parameters
    for (int i = 0; i < MAX_MOTOR; i++)
    {
        pid_motor[i].target_val = 0.0;
        pid_motor[i].pwm_output = 0.0;
        pid_motor[i].err = 0.0;
        pid_motor[i].err_last = 0.0;
        pid_motor[i].err_next = 0.0;
        pid_motor[i].integral = 0.0;

        pid_motor[i].Kp = PID_DEF_KP;
        pid_motor[i].Ki = PID_DEF_KI;
        pid_motor[i].Kd = PID_DEF_KD;
    }

    pid_Yaw.Proportion = PID_YAW_DEF_KP;
    pid_Yaw.Integral = PID_YAW_DEF_KI;
    pid_Yaw.Derivative = PID_YAW_DEF_KD;
}

// Set PID parameters
void PID_Set_Parm(PID_t *pid, float p, float i, float d)
{
    pid->Kp = p; // Set Scale Factor
    pid->Ki = i; // Set integration coefficient
    pid->Kd = d; // Set differential coefficient
}

// Set the target value of PID
void PID_Set_Target(PID_t *pid, float temp_val)
{
    pid->target_val = temp_val; // Set the current target value
}

// Obtain PID target value
float PID_Get_Target(PID_t *pid)
{
    return pid->target_val; // Set the current target value
}

// Incremental PID calculation formula
float PID_Incre_Calc(PID_t *pid, float actual_val)
{
	//Calculate the error between the target value and the actual value
    pid->err = pid->target_val - actual_val;
    //PID algorithm implementation
    pid->pwm_output += pid->Kp * (pid->err - pid->err_next) + pid->Ki * pid->err + pid->Kd * (pid->err - 2 * pid->err_next + pid->err_last);
    //transmission error
    pid->err_last = pid->err_next;
    pid->err_next = pid->err;

    /*Return PWM output value*/

    if (pid->pwm_output > (MOTOR_MAX_PULSE - MOTOR_IGNORE_PULSE))
        pid->pwm_output = (MOTOR_MAX_PULSE - MOTOR_IGNORE_PULSE);
    if (pid->pwm_output < (MOTOR_IGNORE_PULSE - MOTOR_MAX_PULSE))
        pid->pwm_output = (MOTOR_IGNORE_PULSE - MOTOR_MAX_PULSE);

    return pid->pwm_output;
}

//Position PID calculation method
float PID_Location_Calc(PID_t *pid, float actual_val)
{
	/*Calculate the error between the target value and the actual value*/
    pid->err = pid->target_val - actual_val;

    /*Limited closed-loop dead zone*/
    if ((pid->err >= -40) && (pid->err <= 40))
    {
        pid->err = 0;
        pid->integral = 0;
    }

    /*Integral separation, removing the integral effect when the deviation is large*/
    if (pid->err > -1500 && pid->err < 1500)
    {
        pid->integral += pid->err; // error accumulation 误差累积

        /* Limit the integration range to prevent integration saturation 限定积分范围，防止积分饱和 */
        if (pid->integral > 4000)
            pid->integral = 4000;
        else if (pid->integral < -4000)
            pid->integral = -4000;
    }

    /*PID algorithm implementation*/
    pid->output_val = pid->Kp * pid->err +
                      pid->Ki * pid->integral +
                      pid->Kd * (pid->err - pid->err_last);

    /*Error transmission*/
    pid->err_last = pid->err;

    /*Returns the current actual value*/
    return pid->output_val;
}

// PID calculation output value
void PID_Calc_Motor(motor_data_t *motor)
{
    int i;
    // float pid_out[4] = {0};
    // for (i = 0; i < MAX_MOTOR; i++)
    // {
    //     pid_out[i] = PID_Location_Calc(&pid_motor[i], 0);
    //     PID_Set_Motor_Target(i, pid_out[i]);
    // }

    for (i = 0; i < MAX_MOTOR; i++)
    {
        motor->speed_pwm[i] = PID_Incre_Calc(&pid_motor[i], motor->speed_mm_s[i]);
    }
}

// PID calculates one channel separately
float PID_Calc_One_Motor(uint8_t motor_id, float now_speed)
{
    if (motor_id >= MAX_MOTOR)
        return 0;
    return PID_Incre_Calc(&pid_motor[motor_id], now_speed);
}

//Set PID parameters, motor_ Id=4 Set all,=0123 Set the PID parameters of the corresponding motor.
void PID_Set_Motor_Parm(uint8_t motor_id, float kp, float ki, float kd)
{
    if (motor_id > MAX_MOTOR)
        return;

    if (motor_id == MAX_MOTOR)
    {
        for (int i = 0; i < MAX_MOTOR; i++)
        {
            pid_motor[i].Kp = kp;
            pid_motor[i].Ki = ki;
            pid_motor[i].Kd = kd;
        }
    }
    else
    {
        pid_motor[motor_id].Kp = kp;
        pid_motor[motor_id].Ki = ki;
        pid_motor[motor_id].Kd = kd;
    }
}

//Clear PID data
void PID_Clear_Motor(uint8_t motor_id)
{
    if (motor_id > MAX_MOTOR)
        return;

    if (motor_id == MAX_MOTOR)
    {
        for (int i = 0; i < MAX_MOTOR; i++)
        {
            pid_motor[i].pwm_output = 0.0;
            pid_motor[i].err = 0.0;
            pid_motor[i].err_last = 0.0;
            pid_motor[i].err_next = 0.0;
            pid_motor[i].integral = 0.0;
        }
    }
    else
    {
        pid_motor[motor_id].pwm_output = 0.0;
        pid_motor[motor_id].err = 0.0;
        pid_motor[motor_id].err_last = 0.0;
        pid_motor[motor_id].err_next = 0.0;
        pid_motor[motor_id].integral = 0.0;
    }
}

//Set PID target speed in mm/s
void PID_Set_Motor_Target(uint8_t motor_id, float target)
{
    if (motor_id > MAX_MOTOR)
        return;

    if (motor_id == MAX_MOTOR)
    {
        for (int i = 0; i < MAX_MOTOR; i++)
        {
            pid_motor[i].target_val = target;
        }
    }
    else
    {
        pid_motor[motor_id].target_val = target;
    }
}

//Returns an array of PID structures
PID_t *Pid_Get_Motor(void)
{
    return pid_motor;
}

//Reset the target value of yaw angle
void PID_Yaw_Reset(float yaw)
{
    pid_Yaw.SetPoint = yaw;
    pid_Yaw.SumError = 0;
    pid_Yaw.LastError = 0;
    pid_Yaw.PrevError = 0;
}

//Calculate the output value of yaw angle
float PID_Yaw_Calc(float NextPoint)
{
    float dError, Error;
    Error = pid_Yaw.SetPoint - NextPoint;           // deviation
    pid_Yaw.SumError += Error;                      // integral
    dError = pid_Yaw.LastError - pid_Yaw.PrevError; // Current differential
    pid_Yaw.PrevError = pid_Yaw.LastError;
    pid_Yaw.LastError = Error;

    double omega_rad = pid_Yaw.Proportion * Error            // proportional
                       + pid_Yaw.Integral * pid_Yaw.SumError // Integral term
                       + pid_Yaw.Derivative * dError;        // differential term

    if (omega_rad > PI / 6)
        omega_rad = PI / 6;
    if (omega_rad < -PI / 6)
        omega_rad = -PI / 6;
    return omega_rad;
}

// Set parameters for yaw angle PID
void PID_Yaw_Set_Parm(float kp, float ki, float kd)
{
    pid_Yaw.Proportion = kp;
    pid_Yaw.Integral = ki;
    pid_Yaw.Derivative = kd;
}
