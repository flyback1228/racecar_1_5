################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/jy901/JY901_Serial.cpp 

OBJS += \
./Core/Inc/jy901/JY901_Serial.o 

CPP_DEPS += \
./Core/Inc/jy901/JY901_Serial.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/jy901/%.o Core/Inc/jy901/%.su Core/Inc/jy901/%.cyclo: ../Core/Inc/jy901/%.cpp Core/Inc/jy901/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-jy901

clean-Core-2f-Inc-2f-jy901:
	-$(RM) ./Core/Inc/jy901/JY901_Serial.cyclo ./Core/Inc/jy901/JY901_Serial.d ./Core/Inc/jy901/JY901_Serial.o ./Core/Inc/jy901/JY901_Serial.su

.PHONY: clean-Core-2f-Inc-2f-jy901

