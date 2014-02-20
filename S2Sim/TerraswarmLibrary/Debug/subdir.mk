################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ClientConnectionRequest.cpp \
../ClientConnectionResponse.cpp \
../ClientData.cpp \
../DemandNegotiation.cpp \
../GetPrice.cpp \
../MessageEnder.cpp \
../MessageHeader.cpp \
../PriceProposal.cpp \
../SetCurrentPrice.cpp \
../main.cpp 

OBJS += \
./ClientConnectionRequest.o \
./ClientConnectionResponse.o \
./ClientData.o \
./DemandNegotiation.o \
./GetPrice.o \
./MessageEnder.o \
./MessageHeader.o \
./PriceProposal.o \
./SetCurrentPrice.o \
./main.o 

CPP_DEPS += \
./ClientConnectionRequest.d \
./ClientConnectionResponse.d \
./ClientData.d \
./DemandNegotiation.d \
./GetPrice.d \
./MessageEnder.d \
./MessageHeader.d \
./PriceProposal.d \
./SetCurrentPrice.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


