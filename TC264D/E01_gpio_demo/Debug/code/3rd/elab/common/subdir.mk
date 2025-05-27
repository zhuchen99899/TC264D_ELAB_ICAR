################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/3rd/elab/common/elab_assert.c \
../code/3rd/elab/common/elab_common.c \
../code/3rd/elab/common/elab_export.c \
../code/3rd/elab/common/elab_log.c 

OBJS += \
./code/3rd/elab/common/elab_assert.o \
./code/3rd/elab/common/elab_common.o \
./code/3rd/elab/common/elab_export.o \
./code/3rd/elab/common/elab_log.o 

COMPILED_SRCS += \
./code/3rd/elab/common/elab_assert.src \
./code/3rd/elab/common/elab_common.src \
./code/3rd/elab/common/elab_export.src \
./code/3rd/elab/common/elab_log.src 

C_DEPS += \
./code/3rd/elab/common/elab_assert.d \
./code/3rd/elab/common/elab_common.d \
./code/3rd/elab/common/elab_export.d \
./code/3rd/elab/common/elab_log.d 


# Each subdirectory must supply rules for building sources it contributes
code/3rd/elab/common/%.src: ../code/3rd/elab/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fD:/code/NIT_EE_Car/R3/controller/TC264D/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/3rd/elab/common/%.o: ./code/3rd/elab/common/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


