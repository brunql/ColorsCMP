################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lcd/lcd_nokia_3310_frm_brunql.c \
../src/lcd/lcd_nokia_menu.c 

OBJS += \
./src/lcd/lcd_nokia_3310_frm_brunql.o \
./src/lcd/lcd_nokia_menu.o 

C_DEPS += \
./src/lcd/lcd_nokia_3310_frm_brunql.d \
./src/lcd/lcd_nokia_menu.d 


# Each subdirectory must supply rules for building sources it contributes
src/lcd/%.o: ../src/lcd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/media/projects/C_C++/ColorsCMP_EclipseEdition/inc" -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=c99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


