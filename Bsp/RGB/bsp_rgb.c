/*
 * bsp_rgb.c
 */

#include "bsp_rgb.h"
#include "main.h"
/*
 * Turn on the left or right searchlight and display the selected color
 * Select the RGB searchlights that need to be turned on on the left and right sides
 * Choose the color you want to display
 * */
void Set_RGB(car_RGB light, RGB_Color color)
{
	uint8_t Light_RGB = light;
	switch (Light_RGB)
	{
	case RGB_R:
		Set_color_R(color);
		break;
	case RGB_L:
		Set_color_L(color);
		break;
	case RGB_Max:
		Set_color_R(color);
		Set_color_L(color);
		break;
	default:
		RGB_OFF_ALL;
	}
}

/*
 * Turn on the searchlight on the right and show the color
 * Select the color to be displayed
 * */
void Set_color_R(RGB_Color color)
{
	switch (color)
	{
	case red:
		RRGB_SET(set, reset, reset);
		break;
	case green:
		RRGB_SET(reset, set, reset);
		break;
	case blue:
		RRGB_SET(reset, reset, set);
		break;
	case yellow:
		RRGB_SET(set, set, reset);
		break;
	case purple:
		RRGB_SET(set, reset, set);
		break;
	case lake:
		RRGB_SET(reset, set, set);
		break;
	case white:
		RRGB_SET(set, set, set);
		break;
	default:
		RGB_OFF_R;
	}
}

/*
 * Turn on the left searchlight and show colors
 * Select the color to be displayed
 * */
void Set_color_L(RGB_Color color)
{
	switch (color)
	{
	case red:
		LRGB_SET(set, reset, reset);
		break;
	case green:
		LRGB_SET(reset, set, reset);
		break;
	case blue:
		LRGB_SET(reset, reset, set);
		break;
	case yellow:
		LRGB_SET(set, set, 0);
		break;
	case purple:
		LRGB_SET(set, reset, set);
		break;
	case lake:
		LRGB_SET(reset, set, set);
		break;
	case white:
		LRGB_SET(set, set, set);
		break;
	default:
		RGB_OFF_L;
	}
}
