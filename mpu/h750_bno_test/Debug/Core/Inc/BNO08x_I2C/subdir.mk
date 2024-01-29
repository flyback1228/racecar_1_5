################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/BNO08x_I2C/BNO08x_I2C.c 

OBJS += \
./Core/Inc/BNO08x_I2C/BNO08x_I2C.o 

C_DEPS += \
./Core/Inc/BNO08x_I2C/BNO08x_I2C.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/BNO08x_I2C/%.o Core/Inc/BNO08x_I2C/%.su Core/Inc/BNO08x_I2C/%.cyclo: ../Core/Inc/BNO08x_I2C/%.c Core/Inc/BNO08x_I2C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-BNO08x_I2C

clean-Core-2f-Inc-2f-BNO08x_I2C:
	-$(RM) ./Core/Inc/BNO08x_I2C/BNO08x_I2C.cyclo ./Core/Inc/BNO08x_I2C/BNO08x_I2C.d ./Core/Inc/BNO08x_I2C/BNO08x_I2C.o ./Core/Inc/BNO08x_I2C/BNO08x_I2C.su

.PHONY: clean-Core-2f-Inc-2f-BNO08x_I2C

