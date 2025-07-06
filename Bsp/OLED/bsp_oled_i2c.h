#ifndef __BSP_OLED_I2C_H__
#define __BSP_OLED_I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "bsp_oled.h"

#define I2C_Direction_Transmitter ((uint8_t)0x00) // 发送模式 transmit mode
#define I2C_Direction_Receiver ((uint8_t)0x01)    // 接收模式 RECEIVE MODE

/*IIC 事件*/
/* --EV5 : 起始位已发送 Start bit sent...*/
#define I2C_EVENT_MASTER_MODE_SELECT ((uint32_t)0x00030001) /* BUSY, MSL and SB flag */
/* --EV6 : 发送/接收完成 Send/Receive Complete...*/
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ((uint32_t)0x00070082) /* SEND BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ((uint32_t)0x00030002)    /* RECV BUSY, MSL and ADDR flags */
/* Master RECEIVER mode -----------------------------*/
/* --EV7 */
#define I2C_EVENT_MASTER_BYTE_RECEIVED ((uint32_t)0x00030040) /* BUSY, MSL and RXNE flags */
/* Master TRANSMITTER mode --------------------------*/
/* --EV8 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING ((uint32_t)0x00070080) /* TRA, BUSY, MSL, TXE flags */
/* --EV8_2 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED ((uint32_t)0x00070084) /* TRA, BUSY, MSL, TXE and BTF flags */
    /*=========================================================================================*/

    void I2C1_OLED_Init(void);
    uint8_t I2C_CheckEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);
    void I2C_Send7bitAddress(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t I2C_Direction);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
