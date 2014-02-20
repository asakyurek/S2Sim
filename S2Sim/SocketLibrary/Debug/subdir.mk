################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../IPAddress.cpp \
../TCPClient.cpp \
../TCPConnectedClient.cpp \
../TCPServer.cpp \
../ThreadedTCPClient.cpp \
../ThreadedTCPConnectedClient.cpp \
../ThreadedTCPServer.cpp \
../UDPSocket.cpp 

OBJS += \
./IPAddress.o \
./TCPClient.o \
./TCPConnectedClient.o \
./TCPServer.o \
./ThreadedTCPClient.o \
./ThreadedTCPConnectedClient.o \
./ThreadedTCPServer.o \
./UDPSocket.o 

CPP_DEPS += \
./IPAddress.d \
./TCPClient.d \
./TCPConnectedClient.d \
./TCPServer.d \
./ThreadedTCPClient.d \
./ThreadedTCPConnectedClient.d \
./ThreadedTCPServer.d \
./UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/asakyurek/Google Drive/Simulations/ThreadLibrary" -I"/Users/asakyurek/Google Drive/Simulations/SemaphoreLibrary" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


