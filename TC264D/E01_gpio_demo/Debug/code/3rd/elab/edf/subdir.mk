################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/3rd/elab/edf/elab_device.c \
../code/3rd/elab/edf/elab_device_null.c 

OBJS += \
./code/3rd/elab/edf/elab_device.o \
./code/3rd/elab/edf/elab_device_null.o 

COMPILED_SRCS += \
./code/3rd/elab/edf/elab_device.src \
./code/3rd/elab/edf/elab_device_null.src 

C_DEPS += \
./code/3rd/elab/edf/elab_device.d \
./code/3rd/elab/edf/elab_device_null.d 


# Each subdirectory must supply rules for building sources it contributes
code/3rd/elab/edf/%.src: ../code/3rd/elab/edf/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fD:/code/NIT_EE_Car/R3/controller/TC264D/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/3rd/elab/edf/%.o: ./code/3rd/elab/edf/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


