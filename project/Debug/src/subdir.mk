################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/i2c_implementation.c \
../src/main.c 

OBJS += \
./src/_write.o \
./src/i2c_implementation.o \
./src/main.o 

C_DEPS += \
./src/_write.d \
./src/i2c_implementation.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DUSE_FULL_ASSERT -DSTM32F10X_XL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../Inc" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/bps120" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS/gcc_arm_cm3" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f1-stdperiph" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/gd32f10x_standart_peripheral" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/sfm3019-0.1.1" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


