################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/pid/pid.cpp 

OBJS += \
./Core/Inc/pid/pid.o 

CPP_DEPS += \
./Core/Inc/pid/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/pid/%.o Core/Inc/pid/%.su Core/Inc/pid/%.cyclo: ../Core/Inc/pid/%.cpp Core/Inc/pid/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-pid

clean-Core-2f-Inc-2f-pid:
	-$(RM) ./Core/Inc/pid/pid.cyclo ./Core/Inc/pid/pid.d ./Core/Inc/pid/pid.o ./Core/Inc/pid/pid.su

.PHONY: clean-Core-2f-Inc-2f-pid

