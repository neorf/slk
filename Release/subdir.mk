################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../an_packet_protocol.c \
../interfaces.c \
../run.c \
../spatial_packets.c 

OBJS += \
./an_packet_protocol.o \
./interfaces.o \
./run.o \
./spatial_packets.o 

C_DEPS += \
./an_packet_protocol.d \
./interfaces.d \
./run.d \
./spatial_packets.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	armv7a-hardfloat-linux-gnueabi-gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


