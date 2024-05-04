################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_uC/gpio.c \
../HMI_uC/hmi.c \
../HMI_uC/keypad.c \
../HMI_uC/lcd.c \
../HMI_uC/uart.c 

OBJS += \
./HMI_uC/gpio.o \
./HMI_uC/hmi.o \
./HMI_uC/keypad.o \
./HMI_uC/lcd.o \
./HMI_uC/uart.o 

C_DEPS += \
./HMI_uC/gpio.d \
./HMI_uC/hmi.d \
./HMI_uC/keypad.d \
./HMI_uC/lcd.d \
./HMI_uC/uart.d 


# Each subdirectory must supply rules for building sources it contributes
HMI_uC/%.o: ../HMI_uC/%.c HMI_uC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


