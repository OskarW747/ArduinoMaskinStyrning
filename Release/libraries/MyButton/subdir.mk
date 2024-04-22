################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\libraries\MyButton\MyButton.cpp 

LINK_OBJ += \
.\libraries\MyButton\MyButton.cpp.o 

CPP_DEPS += \
.\libraries\MyButton\MyButton.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\MyButton\MyButton.cpp.o: ..\libraries\MyButton\MyButton.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\oskwal05\eclipse\cpp-2024-03\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\Users\oskwal05\eclipse\cpp-2024-03\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.8.6\variants\standard" -I"C:\Users\oskwal05\eclipse\cpp-2024-03\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.8.6\cores\arduino" -I"C:\Users\oskwal05\eclipse\cpp-2024-03\eclipse\arduinoPlugin\libraries\Button\1.0.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


