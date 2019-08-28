################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/APU_Lock_Service.cpp \
../src/SignalHandler.cpp \
../src/domain_socket_Server.cpp \
../src/log_function.cpp \
../src/processcommand.cpp 

OBJS += \
./src/APU_Lock_Service.o \
./src/SignalHandler.o \
./src/domain_socket_Server.o \
./src/log_function.o \
./src/processcommand.o 

CPP_DEPS += \
./src/APU_Lock_Service.d \
./src/SignalHandler.d \
./src/domain_socket_Server.d \
./src/log_function.d \
./src/processcommand.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


