################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/OLED/bsp_oled.c \
../Bsp/OLED/bsp_oled_i2c.c \
../Bsp/OLED/oled_fonts.c 

OBJS += \
./Bsp/OLED/bsp_oled.o \
./Bsp/OLED/bsp_oled_i2c.o \
./Bsp/OLED/oled_fonts.o 

C_DEPS += \
./Bsp/OLED/bsp_oled.d \
./Bsp/OLED/bsp_oled_i2c.d \
./Bsp/OLED/oled_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/OLED/%.o Bsp/OLED/%.su Bsp/OLED/%.cyclo: ../Bsp/OLED/%.c Bsp/OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/Task" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/Key" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/RGB" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/Buzzer" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/PS2" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/delay" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/OLED" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/ultrasonic" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/encoder" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/motor" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/tim" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/freertos" -I"C:/Projet Programmation/GitHub/FreeRTOS car - Copie/FreeRTOS/Bsp/timer_servo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Bsp-2f-OLED

clean-Bsp-2f-OLED:
	-$(RM) ./Bsp/OLED/bsp_oled.cyclo ./Bsp/OLED/bsp_oled.d ./Bsp/OLED/bsp_oled.o ./Bsp/OLED/bsp_oled.su ./Bsp/OLED/bsp_oled_i2c.cyclo ./Bsp/OLED/bsp_oled_i2c.d ./Bsp/OLED/bsp_oled_i2c.o ./Bsp/OLED/bsp_oled_i2c.su ./Bsp/OLED/oled_fonts.cyclo ./Bsp/OLED/oled_fonts.d ./Bsp/OLED/oled_fonts.o ./Bsp/OLED/oled_fonts.su

.PHONY: clean-Bsp-2f-OLED

