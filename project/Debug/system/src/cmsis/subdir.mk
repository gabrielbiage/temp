################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_gd32f10x.c \
../system/src/cmsis/vectors_stm32f10x.c 

OBJS += \
./system/src/cmsis/system_gd32f10x.o \
./system/src/cmsis/vectors_stm32f10x.o 

C_DEPS += \
./system/src/cmsis/system_gd32f10x.d \
./system/src/cmsis/vectors_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DUSE_FULL_ASSERT -DSTM32F10X_XL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../Inc" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/bps120" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS/gcc_arm_cm3" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f1-stdperiph" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/gd32f10x_standart_peripheral" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/sfm3019-0.1.1" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


