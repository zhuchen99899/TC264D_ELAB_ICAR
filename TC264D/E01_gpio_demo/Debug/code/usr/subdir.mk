################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/usr/handle_cmd.c \
../code/usr/serial1_ringbuf_handle.c \
../code/usr/test_export.c 

OBJS += \
./code/usr/handle_cmd.o \
./code/usr/serial1_ringbuf_handle.o \
./code/usr/test_export.o 

COMPILED_SRCS += \
./code/usr/handle_cmd.src \
./code/usr/serial1_ringbuf_handle.src \
./code/usr/test_export.src 

C_DEPS += \
./code/usr/handle_cmd.d \
./code/usr/serial1_ringbuf_handle.d \
./code/usr/test_export.d 


# Each subdirectory must supply rules for building sources it contributes
code/usr/%.src: ../code/usr/%.c code/usr/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fD:/Code/embedded/3.work/ICAR/TC264D/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/usr/%.o: ./code/usr/%.src code/usr/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


