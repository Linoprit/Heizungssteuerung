################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Application/HeizStateMachine/MainMachine.cpp 

OBJS += \
./Application/HeizStateMachine/MainMachine.o 

CPP_DEPS += \
./Application/HeizStateMachine/MainMachine.d 


# Each subdirectory must supply rules for building sources it contributes
Application/HeizStateMachine/%.o: ../Application/HeizStateMachine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Inc" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Drivers/CMSIS/Include" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Framework" -I"/home/harald/eclipse_workspaces/swb/Heizungssteuerung/Application"  -O3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


