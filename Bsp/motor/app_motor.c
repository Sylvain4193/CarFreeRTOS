#include "app_motor.h"

static float speed_lr = 0;
static float speed_fb = 0;
static float speed_spin = 0;

static int speed_L1_setup = 0;
static int speed_L2_setup = 0;
static int speed_R1_setup = 0;
static int speed_R2_setup = 0;

static int g_offset_yaw = 0;
static int16_t g_speed_setup = 0;

//Encoder data before and after 10 milliseconds
int g_Encoder_All_Now[MAX_MOTOR] = {0};
int g_Encoder_All_Last[MAX_MOTOR] = {0};
int g_Encoder_All_Offset[MAX_MOTOR] = {0};

uint8_t g_start_ctrl = 0;

car_data_t car_data;
motor_data_t motor_data;

uint8_t g_yaw_adjust = 0;

int16_t g_car_speed = 200;

int8_t step = 2;
int16_t g_car_boost = 20;

BeepCommand_t beepCmdMotor = {1};
int8_t Beep_Trigger_State_Motor = 0;

static float Motion_Get_Circle_Pulse(void)
{
    return ENCODER_CIRCLE_450;
}

//Only used to display data when added to debugging.
void *Motion_Get_Data(uint8_t index)
{
    if (index == 1)
        return (int *)g_Encoder_All_Now;
    if (index == 2)
        return (int *)g_Encoder_All_Last;
    if (index == 3)
        return (int *)g_Encoder_All_Offset;
    return 0;
}

//Obtain motor speed
void Motion_Get_Motor_Speed(float *speed)
{
    for (int i = 0; i < 4; i++)
    {
        speed[i] = motor_data.speed_mm_s[i];
    }
}

//Set the yaw angle status, and if enabled, refresh the target target angle.
void Motion_Set_Yaw_Adjust(uint8_t adjust)
{
    if (adjust == 0)
    {
        g_yaw_adjust = 0;
    }
    else
    {
        g_yaw_adjust = 1;
    }
    if (g_yaw_adjust)
    {
        // PID_Yaw_Reset
    }
}

//Return to yaw angle adjustment status.
uint8_t Motion_Get_Yaw_Adjust(void)
{
    return g_yaw_adjust;
}

//Control the movement of the car, Motor_ X=[-3600, 3600], if it exceeds the range, it is invalid.
void Motion_Set_Pwm(int16_t Motor_1, int16_t Motor_2, int16_t Motor_3, int16_t Motor_4)
{
    if (Motor_1 >= -MOTOR_MAX_PULSE && Motor_1 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M1, Motor_1);
    }
    if (Motor_2 >= -MOTOR_MAX_PULSE && Motor_2 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M2, Motor_2);
    }
    if (Motor_3 >= -MOTOR_MAX_PULSE && Motor_3 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M3, Motor_3);
    }
    if (Motor_4 >= -MOTOR_MAX_PULSE && Motor_4 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M4, Motor_4);
    }
}

// Car Stop
void Motion_Stop(uint8_t brake)
{
    Motion_Set_Speed(0, 0, 0, 0);
    PID_Clear_Motor(MAX_MOTOR);
    g_start_ctrl = 0;
    g_yaw_adjust = 0;
    Motor_Stop(brake);
}

//speed_mX=[-1000,1000],Unit: mm/s
void Motion_Set_Speed(int16_t speed_m1, int16_t speed_m2, int16_t speed_m3, int16_t speed_m4)
{
    g_start_ctrl = 1;
    motor_data.speed_set[0] = speed_m1;
    motor_data.speed_set[1] = speed_m2;
    motor_data.speed_set[2] = speed_m3;
    motor_data.speed_set[3] = speed_m4;
    for (uint8_t i = 0; i < MAX_MOTOR; i++)
    {
        PID_Set_Motor_Target(i, motor_data.speed_set[i] * 1.0);
    }
}

//Increase yaw angle to calibrate the direction of the car's movement
void Motion_Yaw_Calc(float yaw)
{
    Wheel_Yaw_Calc(yaw);
}

void Wheel_Yaw_Calc(float yaw)
{
    float yaw_offset = PID_Yaw_Calc(yaw);
    g_offset_yaw = yaw_offset * g_speed_setup;

    int speed_L1 = speed_L1_setup - g_offset_yaw;
    int speed_L2 = speed_L2_setup - g_offset_yaw;
    int speed_R1 = speed_R1_setup + g_offset_yaw;
    int speed_R2 = speed_R2_setup + g_offset_yaw;

    if (speed_L1 > 1000)
        speed_L1 = 1000;
    if (speed_L1 < -1000)
        speed_L1 = -1000;
    if (speed_L2 > 1000)
        speed_L2 = 1000;
    if (speed_L2 < -1000)
        speed_L2 = -1000;
    if (speed_R1 > 1000)
        speed_R1 = 1000;
    if (speed_R1 < -1000)
        speed_R1 = -1000;
    if (speed_R2 > 1000)
        speed_R2 = 1000;
    if (speed_R2 < -1000)
        speed_R2 = -1000;
    Motion_Set_Speed(speed_L1, speed_L2, speed_R1, speed_R2);
}

//Read the current speed of each wheel from the encoder, in mm/s
void Motion_Get_Speed(car_data_t *car)
{
    int i = 0;
    float speed_mm[MAX_MOTOR] = {0};
    float circle_mm = Motion_Get_Circle_MM();
    float circle_pulse = Motion_Get_Circle_Pulse();
    float robot_APB = Motion_Get_APB();

    Motion_Get_Encoder();

    //Calculate the wheel speed in mm/s.
    for (i = 0; i < 4; i++)
    {
        speed_mm[i] = (g_Encoder_All_Offset[i]) * 100 * circle_mm / circle_pulse;
    }

    car->Vx = (speed_mm[0] + speed_mm[1] + speed_mm[2] + speed_mm[3]) / 4;
    car->Vy = -(speed_mm[0] - speed_mm[1] - speed_mm[2] + speed_mm[3]) / 4;
    car->Vz = -(speed_mm[0] + speed_mm[1] - speed_mm[2] - speed_mm[3]) / 4.0f / robot_APB * 1000;

    if (g_start_ctrl)
    {
        for (i = 0; i < MAX_MOTOR; i++)
        {
            motor_data.speed_mm_s[i] = speed_mm[i];
        }
        PID_Calc_Motor(&motor_data);
    }
}

//Returns half of the sum of the current wheel spacing of the small car
float Motion_Get_APB(void)
{
    return STM32Car_APB;
}

//Returns the number of millimeters the current car wheel has rotated once
float Motion_Get_Circle_MM(void)
{
    return MECANUM_CIRCLE_MM;
}

//Obtain encoder data and calculate the number of deviation pulses
void Motion_Get_Encoder(void)
{
    Encoder_Get_ALL(g_Encoder_All_Now);

    for (uint8_t i = 0; i < MAX_MOTOR; i++)
    {
    	//Record the number of pulses between two test times
        g_Encoder_All_Offset[i] = g_Encoder_All_Now[i] - g_Encoder_All_Last[i];
        //Record Last Encoder Data
        g_Encoder_All_Last[i] = g_Encoder_All_Now[i];
    }
}

//Control the movement of the car
void Motion_Ctrl(int16_t V_x, int16_t V_y, int16_t V_z)
{
    wheel_Ctrl(V_x, V_y, V_z);
}

void Motion_Ctrl_State(uint8_t state, int16_t speed, uint8_t adjust)
{
    int16_t input_speed = speed * 10;
    wheel_State_YAW(state, input_speed, adjust);
}

//Control the movement status of the car.
//Speed control: speed=0-1000.
//Yaw angle adjustment motion: adjust=1 on,=0 not on.
void wheel_State_YAW(uint8_t state, int16_t speed, uint8_t adjust)
{
    Motion_Set_Yaw_Adjust(adjust);
    g_speed_setup = speed;
    switch (state)
    {
    case MOTION_STOP:
        g_speed_setup = 0;
        Motion_Stop(speed == 0 ? STOP_FREE : STOP_BRAKE);
        break;
    case MOTION_RUN:
        wheel_Ctrl(speed, 0, 0);
        break;
    case MOTION_BACK:
        wheel_Ctrl(-speed, 0, 0);
        break;
    case MOTION_LEFT:
    	wheel_Ctrl(speed/2, 0, -speed*2);
        break;
    case MOTION_RIGHT:
    	wheel_Ctrl(speed/2, 0, speed*2);

        break;
    case MOTION_SPIN_LEFT:
        Motion_Set_Yaw_Adjust(0);
        wheel_Ctrl(0, 0, speed * 5);
        break;
    case MOTION_SPIN_RIGHT:
        Motion_Set_Yaw_Adjust(0);
        wheel_Ctrl(0, 0, -speed * 5);
        break;
    case MOTION_BRAKE:
        Motion_Stop(STOP_BRAKE);
        break;
    default:
        break;
    }
}

//Control the movement status of the car.
//Speed control: speed=0-1000.
void wheel_State(uint8_t state, int16_t speed)
{
    g_speed_setup = speed;
    switch (state)
    {
    case MOTION_STOP:
        g_speed_setup = 0;
        Motion_Stop(speed == 0 ? STOP_FREE : STOP_BRAKE);
        break;
    case MOTION_RUN:
        wheel_Ctrl(speed, 0, 0);
        break;
    case MOTION_BACK:
        wheel_Ctrl(-speed, 0, 0);
        break;
    case MOTION_LEFT:
    	wheel_Ctrl(speed/2, 0, -speed*2);
        break;
    case MOTION_RIGHT:
    	wheel_Ctrl(speed/2, 0, speed*2);

        break;
    case MOTION_SPIN_LEFT:
        Motion_Set_Yaw_Adjust(0);
        wheel_Ctrl(0, 0, speed * 5);
        break;
    case MOTION_SPIN_RIGHT:
        Motion_Set_Yaw_Adjust(0);
        wheel_Ctrl(0, 0, -speed * 5);
        break;
    case MOTION_BRAKE:
        Motion_Stop(STOP_BRAKE);
        break;
    default:
        break;
    }
}


void wheel_Ctrl(int16_t V_x, int16_t V_y, int16_t V_z)
{
    float robot_APB = Motion_Get_APB();
//    speed_lr = -V_y;
    speed_lr = 0;
    speed_fb = V_x;
    speed_spin = (V_z / 1000.0f) * robot_APB;
    if (V_x == 0 && V_y == 0 && V_z == 0)
    {
        Motion_Stop(STOP_BRAKE);
        return;
    }

    speed_L1_setup = speed_fb + speed_lr + speed_spin;
    speed_L2_setup = speed_fb - speed_lr + speed_spin;
    speed_R1_setup = speed_fb - speed_lr - speed_spin;
    speed_R2_setup = speed_fb + speed_lr - speed_spin;

    if (speed_L1_setup > 1000)
        speed_L1_setup = 1000;
    else if (speed_L1_setup < -1000)
        speed_L1_setup = -1000;
    if (speed_L2_setup > 1000)
        speed_L2_setup = 1000;
    else if (speed_L2_setup < -1000)
        speed_L2_setup = -1000;
    if (speed_R1_setup > 1000)
        speed_R1_setup = 1000;
    else if (speed_R1_setup < -1000)
        speed_R1_setup = -1000;
    if (speed_R2_setup > 1000)
        speed_R2_setup = 1000;
    else if (speed_R2_setup < -1000)
        speed_R2_setup = -1000;

    Motion_Set_Speed(speed_L1_setup, speed_L2_setup, speed_R1_setup, speed_R2_setup);
}

//Motion control handle, called every 10ms, mainly processing speed related data
void Motion_Handle(void)
{
    Motion_Get_Speed(&car_data);

    if (g_start_ctrl)
    {
        Motion_Set_Pwm(motor_data.speed_pwm[0], motor_data.speed_pwm[1], motor_data.speed_pwm[2], motor_data.speed_pwm[3]);
    }
}

void updateMotorState(MotorCommand_t motorCmd){

	if(motorCmd.L2_pressed == motorCmd.R2_pressed){
		g_car_speed = 0;
		wheel_State(MOTION_STOP, g_car_speed);
	}
	else{
		//Gestion de la vitesse
		if (motorCmd.R2_pressed == 1){ 		      // Forward
			if (g_car_speed < 200)
				g_car_speed = 200;
			else
			{
				if (motorCmd.R1_pressed == 1)     //Boost Only Forward
					step = g_car_boost;
				g_car_speed = g_car_speed + step;
			}
		}

		else if (motorCmd.L2_pressed == 1){       // Backward
			if (g_car_speed > -200)
				g_car_speed = -200;
			else
				g_car_speed = g_car_speed - step;
		}
		//Gestion des commandes
		if (motorCmd.joystickX < 125){
			if (motorCmd.L1_pressed == 1)
				wheel_State(MOTION_SPIN_RIGHT, g_car_speed); //Right and Left are inverse here, bc it's more natural
			else
				wheel_State(MOTION_LEFT, g_car_speed);		//Left
		}
		else if (motorCmd.joystickX > 150){
			if (motorCmd.L1_pressed == 1){
				wheel_State(MOTION_SPIN_LEFT, g_car_speed);  //Right and Left are inverse here, bc it's more natural
			}
			else{
				wheel_State(MOTION_RIGHT, g_car_speed);		//Right
			}
		}
		else { //Forward or Backward
			wheel_State(MOTION_RUN, g_car_speed);
		}
	}
	// Need to be fix :
	// If L2 pressed and hold then green pressed and green release do not stop Beep
	// Maybe need to be fix in task.c
	if( (g_car_speed < 0 && Beep_Trigger_State_Motor == 0 && motorCmd.L1_pressed == 0)
		|| ((motorCmd.L2_pressed == 0 || motorCmd.L1_pressed == 1) && Beep_Trigger_State_Motor == 1))
	{
		xQueueSend(myQueue_controlMotor, &beepCmdMotor, 0);
		Beep_Trigger_State_Motor = !Beep_Trigger_State_Motor;

	}

}
