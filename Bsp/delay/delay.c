#include "delay.h"

/*
 * Redefinition of the delay() function because:
 * - HAL does not start SysTick since it is based on TIM1 (see NVIC configuration)
 * - HAL is not precise enough for microsecond delays
 *
 * This delay is used for PS2 configuration
 * It becomes usable throughout the code, but note that it is blocking.
 */
void Delay_Init(void)
{
    // Activate count DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
 * Timing us with DWT
 */
void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);  // cycles pour us
    while ((DWT->CYCCNT - start) < ticks);
}

void Delay_Ms(uint16_t ms)
{
    while (ms--)
        delay_us(1000);
}
