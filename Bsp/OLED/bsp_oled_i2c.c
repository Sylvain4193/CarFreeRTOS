#include "bsp_oled_i2c.h"

void I2C1_OLED_Init(void)
{
  LL_I2C_Enable(I2C1); // Start i2c 启动i2c
}

void I2C_Send7bitAddress(I2C_TypeDef *I2Cx, uint8_t Address, uint8_t I2C_Direction)
{
  /* Test on the direction to set/reset the read/write bit */
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read */
    Address |= I2C_OAR1_ADD0;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= (uint8_t) ~((uint8_t)I2C_OAR1_ADD0);
  }
  /* Send the address */
  I2Cx->DR = Address;
}

uint8_t I2C_CheckEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  uint8_t status = 0;

  /* Read the I2Cx status register */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & ((uint32_t)0x00FFFFFF);

  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = 1;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = 0;
  }
  /* Return status */
  return status;
}
