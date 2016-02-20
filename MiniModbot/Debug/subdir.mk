################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Encoder.cpp \
../Motor.cpp \
../PS4.cpp \
../main.cpp 

OBJS += \
./Encoder.o \
./Motor.o \
./PS4.o \
./main.o 

CPP_DEPS += \
./Encoder.d \
./Motor.d \
./PS4.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/Users/Claire/Documents/iotdk-ide-mac/devkit-x86/sysroots/i586-poky-linux/usr/include/ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


