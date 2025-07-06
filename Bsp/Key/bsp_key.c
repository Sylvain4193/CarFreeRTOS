#include "bsp_key.h"

uint16_t g_key1_long_press = 0;
uint16_t g_key2_long_press = 0;
uint16_t g_key3_long_press = 0;

/*
 *Determine whether the button is pressed, press to return KEY_PRESS, release to return KEY_RELEASE
 * */
static uint8_t Key1_is_Press(void)
{
	if (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin))
	{
		return KEY_PRESS; // If the key is pressed, return KEY_PRESS
	}
	return KEY_RELEASE; // If the key is released, return KEY_RELEASE
}

/*
 *Determine whether the button is pressed, press to return KEY_PRESS, release to return KEY_RELEASE
 * */

static uint8_t Key2_is_Press(void)
{
	if (!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin))
	{
		return KEY_PRESS; // KEY_PRESS
	}
	return KEY_RELEASE; // KEY_RELEASE
}

/*
 *Determine whether the button is pressed, press to return KEY_PRESS, release to return KEY_RELEASE
 * */

static uint8_t Key3_is_Press(void)
{
	if (!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin))
	{
		return KEY_PRESS; // KEY_PRESS
	}
	return KEY_RELEASE; // KEY_RELEASE
}

/*
 * Read the long press status of button K1, return 1 when the accumulated long press time is reached,
 *  and return 0 when the accumulated long press time is not reached.
 * timeout is the set time length, the unit is seconds
 * */

uint8_t Key1_Long_Press(uint16_t timeout)
{
	if (g_key1_long_press > 0)
	{
		if (g_key1_long_press < timeout * 100 + 2)
		{
			g_key1_long_press++;
			if (g_key1_long_press == timeout * 100 + 2)
			{
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

/*
 * Read the long press status of button K2, return 1 when the accumulated long press time is reached,
 * and return 0 when the accumulated long press time is not reached.
 * timeout is the set time length, the unit is seconds
 * */

uint8_t Key2_Long_Press(uint16_t timeout)
{
	if (g_key2_long_press > 0)
	{
		if (g_key2_long_press < timeout * 100 + 2)
		{
			g_key2_long_press++;
			if (g_key2_long_press == timeout * 100 + 2)
			{
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

/*
 * Read the status of button K1, return 1 when pressed, and return 0 when released.
 * mode: setting mode, 0: Press to return to 1 all the time; 1: Press to return to 1 only once
 * */

uint8_t Key1_State(uint8_t mode)
{
	static uint16_t key1_state = 0;

	if (Key1_is_Press() == KEY_PRESS)
	{
		if (key1_state < (mode + 1) * 2)
		{
			key1_state++;
		}
	}
	else
	{
		key1_state = 0;
		g_key1_long_press = 0;
	}
	if (key1_state == 2)
	{
		g_key1_long_press = 1;
		return KEY_PRESS;
	}
	return KEY_RELEASE;
}

/*
 * Read the status of button K2, return 1 when pressed, and return 0 when released.
 * mode: setting mode, 0: Press to return to 1 all the time; 1: Press to return to 1 only once
 * */

uint8_t Key2_State(uint8_t mode)
{
	static uint16_t key2_state = 0;

	if (Key2_is_Press() == KEY_PRESS)
	{
		if (key2_state < (mode + 1) * 2)
		{
			key2_state++;
		}
	}
	else
	{
		key2_state = 0;
		g_key2_long_press = 0;
	}
	if (key2_state == 2)
	{
		g_key2_long_press = 1;
		return KEY_PRESS;
	}
	return KEY_RELEASE;
}

/*
 * Read the long press status of button K3, return 1 when the accumulated long press time is reached, and return 0 when the accumulated long press time is not reached.
 * timeout is the set time length, the unit is seconds
 * */

uint8_t Key3_Long_Press(uint16_t timeout)
{
	if (g_key3_long_press > 0)
	{
		if (g_key3_long_press < timeout * 100 + 2)
		{
			g_key3_long_press++;
			if (g_key3_long_press == timeout * 100 + 2)
			{
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

/*
 * Read the status of button K3, return 1 when pressed, and return 0 when released.
 * mode: setting mode, 0: Press to return to 1 all the time; 1: Press to return to 1 only once
 * */

uint8_t Key3_State(uint8_t mode)
{
	static uint16_t key3_state = 0;

	if (Key3_is_Press() == KEY_PRESS)
	{
		if (key3_state < (mode + 1) * 2)
		{
			key3_state++;
		}
	}
	else
	{
		key3_state = 0;
		g_key3_long_press = 0;
	}
	if (key3_state == 2)
	{
		g_key3_long_press = 1;
		return KEY_PRESS;
	}
	return KEY_RELEASE;
}
