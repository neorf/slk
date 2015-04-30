################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rs232/rs232.c 

OBJS += \
./rs232/rs232.o 

C_DEPS += \
./rs232/rs232.d 


# Each subdirectory must supply rules for building sources it contributes
rs232/%.o: ../rs232/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	armv7a-hardfloat-linux-gnueabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


