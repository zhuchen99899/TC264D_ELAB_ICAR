################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/mcu/driver/drv_pin.c \
../code/mcu/driver/drv_uart.c 

OBJS += \
./code/mcu/driver/drv_pin.o \
./code/mcu/driver/drv_uart.o 

COMPILED_SRCS += \
./code/mcu/driver/drv_pin.src \
./code/mcu/driver/drv_uart.src 

C_DEPS += \
./code/mcu/driver/drv_pin.d \
./code/mcu/driver/drv_uart.d 


# Each subdirectory must supply rules for building sources it contributes
code/mcu/driver/%.src: ../code/mcu/driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fD:/code/NIT_EE_Car/R3/controller/TC264D/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/mcu/driver/%.o: ./code/mcu/driver/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


