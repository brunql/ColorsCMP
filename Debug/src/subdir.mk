################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SnakeGame.c \
../src/algorithm.c \
../src/debug.c \
../src/led_driver.c \
../src/main.c \
../src/uint8_16_to_string.c \
../src/usb.c 

OBJS += \
./src/SnakeGame.o \
./src/algorithm.o \
./src/debug.o \
./src/led_driver.o \
./src/main.o \
./src/uint8_16_to_string.o \
./src/usb.o 

C_DEPS += \
./src/SnakeGame.d \
./src/algorithm.d \
./src/debug.d \
./src/led_driver.d \
./src/main.d \
./src/uint8_16_to_string.d \
./src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/media/projects/C_C++/ColorsCMP_EclipseEdition/inc" -I"/media/projects/C_C++/ColorsCMP_EclipseEdition/usbdrv/inc" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=c99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


