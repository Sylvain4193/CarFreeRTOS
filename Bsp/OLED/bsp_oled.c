#include "bsp_oled.h"

/* Write command */
#define SSD1306_WRITECOMMAND(command) I2C_WriteByte(0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data) I2C_WriteByte(0x40, (data))
/* Absolute value */

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef struct
{
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

//  Write data to OLED
void I2C_WriteByte(unsigned char addr, unsigned char data)
{
    while (LL_I2C_IsActiveFlag_BUSY(OLED_I2C))
        ;                                    // Determine if the bus is busy
    LL_I2C_GenerateStartCondition(OLED_I2C); // START 1
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        ;                                                                   // Read SR1->SB bit to determine whether the start bit is sent
    I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter); // Send from device address
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;                                 // Waiting for EV6 to complete; Read SR1
    LL_I2C_TransmitData8(OLED_I2C, addr); // Register address of the slave device
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    LL_I2C_TransmitData8(OLED_I2C, data); // send data
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    LL_I2C_GenerateStopCondition(OLED_I2C);
}

// Function function: oled initialization
// Incoming parameter: None
void OLED_Init(void)
{
    I2C1_OLED_Init();
    HAL_Delay(100);

    SSD1306_WRITECOMMAND(0xae); // display off
    SSD1306_WRITECOMMAND(0xa6); // Set Normal Display (default)
    SSD1306_WRITECOMMAND(0xAE); // DISPLAYOFF
    SSD1306_WRITECOMMAND(0xD5); // SETDISPLAYCLOCKDIV
    SSD1306_WRITECOMMAND(0x80); // the suggested ratio 0x80
    SSD1306_WRITECOMMAND(0xA8); // SSD1306_SETMULTIPLEX
    SSD1306_WRITECOMMAND(0x1F);
    SSD1306_WRITECOMMAND(0xD3);       // SETDISPLAYOFFSET
    SSD1306_WRITECOMMAND(0x00);       // no offset
    SSD1306_WRITECOMMAND(0x40 | 0x0); // SETSTARTLINE
    SSD1306_WRITECOMMAND(0x8D);       // CHARGEPUMP
    SSD1306_WRITECOMMAND(0x14);       // 0x014 enable, 0x010 disable
    SSD1306_WRITECOMMAND(0x20);       // com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5),
    SSD1306_WRITECOMMAND(0x02);       // 0x12 //128x32 OLED: 0x002,  128x32 OLED 0x012
    SSD1306_WRITECOMMAND(0xa1);       // segment remap a0/a1
    SSD1306_WRITECOMMAND(0xc8);       // c0: scan dir normal, c8: reverse
    SSD1306_WRITECOMMAND(0xda);
    SSD1306_WRITECOMMAND(0x02); // com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
    SSD1306_WRITECOMMAND(0x81);
    SSD1306_WRITECOMMAND(0xcf); // [2] set contrast control
    SSD1306_WRITECOMMAND(0xd9);
    SSD1306_WRITECOMMAND(0xf1); // [2] pre-charge period 0x022/f1
    SSD1306_WRITECOMMAND(0xdb);
    SSD1306_WRITECOMMAND(0x40); // vcomh deselect level
    SSD1306_WRITECOMMAND(0x2e); // Disable scroll
    SSD1306_WRITECOMMAND(0xa4); // output ram to display
    SSD1306_WRITECOMMAND(0xa6); // none inverted normal display mode
    SSD1306_WRITECOMMAND(0xaf); // display on

    /* Clear screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);

    /* Update screen */
    SSD1306_UpdateScreen();

    /* Set default values */
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    /* Initialized OK */
    SSD1306.Initialized = 1;
}

// Function function: OLED screen update display
// Incoming parameter: None
void SSD1306_UpdateScreen(void)
{
    uint8_t m, n;

    for (m = 0; m < 8; m++)
    {
        SSD1306_WRITECOMMAND(0xB0 + m);
        SSD1306_WRITECOMMAND(0x00);
        SSD1306_WRITECOMMAND(0x10);

        for (n = 0; n < SSD1306_WIDTH; n++)
        {
            SSD1306_WRITEDATA(SSD1306_Buffer[n + SSD1306_WIDTH * m]);
        }
    }
}

// Function function: OLED screen is cleared, but the display is not refreshed
// Incoming parameter: color: SSD1306_ COLOR_ BLACK SSD1306_ COLOR_ WHITE
void SSD1306_Fill(SSD1306_COLOR_t color)
{
    /* Set memory */
    memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        return; // Error, out of range
    }

    /* Check if pixels are inverted*/
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR_t)!color;
    }

    /* set color */
    if (color == SSD1306_COLOR_WHITE)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

// Function function: Set the current cursor
// Incoming parameter: x: abscissa y: ordinate
void SSD1306_GotoXY(uint16_t x, uint16_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef_t *Font, SSD1306_COLOR_t color)
{
    uint32_t i, b, j;

    if (
        SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight))
    {
        return 0; // Error, out of range
    }

    for (i = 0; i < Font->FontHeight; i++)
    {
        b = Font->data[(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)color);
            }
            else
            {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }

    /* Increase pointer */
    SSD1306.CurrentX += Font->FontWidth;

    /* Return character written */
    return ch;
}

char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color)
{
    /* Write characters */
    while (*str)
    {
        /* Write character by character */
        if (SSD1306_Putc(*str, Font, color) != *str)
        {
            /* Return error */
            return *str;
        }

        /* Increase string pointer */
        str++;
    }

    /* Everything OK, zero should be returned */
    return *str;
}

// Wake OLED from sleep
void OLED_ON(void)
{
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x14);
    SSD1306_WRITECOMMAND(0xAF);
}

//  Let OLED sleep - In sleep mode, OLED power consumption is less than 10uA
void OLED_OFF(void)
{
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x10);
    SSD1306_WRITECOMMAND(0xAE);
}

/* OLED Clear Screen */
void OLED_Clear(void)
{
    SSD1306_Fill(SSD1306_COLOR_BLACK);
}

/*Refresh OLED screen */
void OLED_Refresh(void)
{
    SSD1306_UpdateScreen();
}

/* Write Characters */
void OLED_Draw_String(char *data, uint8_t x, uint8_t y, bool clear, bool refresh)
{
    if (clear)
        OLED_Clear();
    SSD1306_GotoXY(x, y);
    SSD1306_Puts(data, &Font_7x10, SSD1306_COLOR_WHITE);
    if (refresh)
        OLED_Refresh();
}

/* Write a line of characters */
void OLED_Draw_Line(char *data, uint8_t line, bool clear, bool refresh)
{
    if (line > 0 && line <= 3)
    {
        OLED_Draw_String(data, 0, 10 * (line - 1), clear, refresh);
    }
}

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp;

    /* Check for overflow */
    if (x0 >= SSD1306_WIDTH)
    {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH)
    {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT)
    {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT)
    {
        y1 = SSD1306_HEIGHT - 1;
    }

    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = ((dx > dy) ? dx : -dy) / 2;

    if (dx == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Vertical line */
        for (i = y0; i <= y1; i++)
        {
            SSD1306_DrawPixel(x0, i, c);
        }

        /* Return from function */
        return;
    }

    if (dy == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Horizontal line */
        for (i = x0; i <= x1; i++)
        {
            SSD1306_DrawPixel(i, y0, c);
        }

        /* Return from function */
        return;
    }

    while (1)
    {
        SSD1306_DrawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// Just like the Rocket League video game.
void OLED_QuickChat(uint32_t firstPad, uint32_t secondPad){
	switch(firstPad)
	{
	case PSB_PAD_UP:
		switch(secondPad)
		{
		case PSB_PAD_UP:
			OLED_Draw_Line("Je l'ai !", 1, true, true);
			break;
		case PSB_PAD_RIGHT:
			OLED_Draw_Line("Besoin de turbo !", 1, true, true);
			break;
		case PSB_PAD_DOWN:
			OLED_Draw_Line("Prends-le !", 1, true, true);
			break;
		case PSB_PAD_LEFT:
			OLED_Draw_Line("Je défends.", 1, true, true);
			break;
		default:
			break;
		}
		break;
	case PSB_PAD_RIGHT:
		switch(secondPad)
		{
		case PSB_PAD_UP:
			OLED_Draw_Line("Beau tir !", 1, true, true);
			break;
		case PSB_PAD_RIGHT:
			OLED_Draw_Line("Belle passe !", 1, true, true);
			break;
		case PSB_PAD_DOWN:
			OLED_Draw_Line("Merci !", 1, true, true);
			break;
		case PSB_PAD_LEFT:
			OLED_Draw_Line("Quel arrêt !", 1, true, true);
			break;
		default:
			break;
		}
		break;
	case PSB_PAD_DOWN:
		switch(secondPad)
		{
		case PSB_PAD_UP:
			OLED_Draw_Line("Joli démo !", 1, true, true);
			break;
		case PSB_PAD_RIGHT:
			OLED_Draw_Line("La feinte.", 1, true, true);
			break;
		case PSB_PAD_DOWN:
			OLED_Draw_Line("Quelle intensité !", 1, true, true);
			break;
		case PSB_PAD_LEFT:
			OLED_Draw_Line("En approche !", 1, true, true);
			break;
		default:
			break;
		}
		break;
	case PSB_PAD_LEFT:
		switch(secondPad)
		{
		case PSB_PAD_UP:
			OLED_Draw_Line("Ok.", 1, true, true);
			break;
		case PSB_PAD_RIGHT:
			OLED_Draw_Line("Sauvage !", 1, true, true);
			break;
		case PSB_PAD_DOWN:
			OLED_Draw_Line("Pas de problème.", 1, true, true);
			break;
		case PSB_PAD_LEFT:
			OLED_Draw_Line("Pardon...", 1, true, true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

