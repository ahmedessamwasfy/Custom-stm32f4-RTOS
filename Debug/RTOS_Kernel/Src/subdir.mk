################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../RTOS_Kernel/Src/Rtos_asm.s \
../RTOS_Kernel/Src/Rtos_svc_calls.s 

C_SRCS += \
../RTOS_Kernel/Src/HelpFun.c \
../RTOS_Kernel/Src/Rtos.c \
../RTOS_Kernel/Src/Rtos_list.c \
../RTOS_Kernel/Src/Rtos_mailbox.c \
../RTOS_Kernel/Src/Rtos_mutex.c \
../RTOS_Kernel/Src/Rtos_semaphore.c \
../RTOS_Kernel/Src/Rtos_thread.c 

OBJS += \
./RTOS_Kernel/Src/HelpFun.o \
./RTOS_Kernel/Src/Rtos.o \
./RTOS_Kernel/Src/Rtos_asm.o \
./RTOS_Kernel/Src/Rtos_list.o \
./RTOS_Kernel/Src/Rtos_mailbox.o \
./RTOS_Kernel/Src/Rtos_mutex.o \
./RTOS_Kernel/Src/Rtos_semaphore.o \
./RTOS_Kernel/Src/Rtos_svc_calls.o \
./RTOS_Kernel/Src/Rtos_thread.o 

S_DEPS += \
./RTOS_Kernel/Src/Rtos_asm.d \
./RTOS_Kernel/Src/Rtos_svc_calls.d 

C_DEPS += \
./RTOS_Kernel/Src/HelpFun.d \
./RTOS_Kernel/Src/Rtos.d \
./RTOS_Kernel/Src/Rtos_list.d \
./RTOS_Kernel/Src/Rtos_mailbox.d \
./RTOS_Kernel/Src/Rtos_mutex.d \
./RTOS_Kernel/Src/Rtos_semaphore.d \
./RTOS_Kernel/Src/Rtos_thread.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS_Kernel/Src/%.o: ../RTOS_Kernel/Src/%.c RTOS_Kernel/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../RTOS_Kernel/Inc -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ahmed Wasfy.DESKTOP-8255SRK/STM32CubeIDE/workspace_1.7.0/Custom_RTOS/RTOS_Kernel/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
RTOS_Kernel/Src/%.o: ../RTOS_Kernel/Src/%.s RTOS_Kernel/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/Ahmed Wasfy.DESKTOP-8255SRK/STM32CubeIDE/workspace_1.7.0/Custom_RTOS/RTOS_Kernel/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

