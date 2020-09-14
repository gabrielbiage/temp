################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/freeRTOS/croutine.c \
../system/src/freeRTOS/event_groups.c \
../system/src/freeRTOS/list.c \
../system/src/freeRTOS/queue.c \
../system/src/freeRTOS/stream_buffer.c \
../system/src/freeRTOS/tasks.c \
../system/src/freeRTOS/timers.c 

OBJS += \
./system/src/freeRTOS/croutine.o \
./system/src/freeRTOS/event_groups.o \
./system/src/freeRTOS/list.o \
./system/src/freeRTOS/queue.o \
./system/src/freeRTOS/stream_buffer.o \
./system/src/freeRTOS/tasks.o \
./system/src/freeRTOS/timers.o 

C_DEPS += \
./system/src/freeRTOS/croutine.d \
./system/src/freeRTOS/event_groups.d \
./system/src/freeRTOS/list.d \
./system/src/freeRTOS/queue.d \
./system/src/freeRTOS/stream_buffer.d \
./system/src/freeRTOS/tasks.d \
./system/src/freeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/freeRTOS/%.o: ../system/src/freeRTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DUSE_FULL_ASSERT -DSTM32F10X_XL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../Inc" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/bps120" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS/gcc_arm_cm3" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/freeRTOS" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f1-stdperiph" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/system/Inc/gd32f10x_standart_peripheral" -I"/home/gabriel/Desktop/Workspaces/eclipse/project/Inc/sfm3019-0.1.1" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


