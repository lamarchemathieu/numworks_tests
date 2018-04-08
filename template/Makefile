

NUM ?= 0
TYPE ?= D

VERSION = $(shell cat version.txt).$(shell date '+%y%j').$(TYPE)

NAME ?= output

FULLNAME = $(NAME)-$(VERSION)-$(NUM)

$(info Building : $(FULLNAME))

ELF = $(FULLNAME).elf
HEX = $(FULLNAME).hex
BIN = $(FULLNAME).bin
LST = $(FULLNAME).lst

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

SRCS =	$(wildcard sources/*.c) \
		$(wildcard ../stm32_libraries/sources/*.c) \
		$(wildcard ../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_*.c) \
		../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/system_stm32l0xx.c

SRCS_ASM = ../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc/startup_stm32l051xx.s

		
INCLUDES =	-I./include \
			-I../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/CMSIS/Include \
			-I../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/CMSIS/Device/ST/STM32L0xx/Include \
			-I../stm32_sdk/STM32Cube_FW_L0_V1.7.0/Drivers/STM32L0xx_HAL_Driver/Inc \
			-I../stm32_libraries/include
OBJS = $(SRCS:.c=.o) $(SRCS_ASM:.s=.o)

CFLAGS = $(INCLUDES) -std=c99 -g -ffunction-sections -Os -mcpu=cortex-m0plus -mthumb -DSTM32L051xx -DUSE_FULL_LL_DRIVER -DARM_MATH_CM0PLUS -DDEBUG=1 -DVERSION="\"$(VERSION)\""
ASFLAGS = $(INCLUDES) -g -mcpu=cortex-m0plus
LDSCRIPT = ../stm32_sdk/LinkerScripts/STM32L051K8_flash.lds
LDFLAGS = -T$(LDSCRIPT) -Wl,-gc-sections -mcpu=cortex-m0plus -mthumb

all : $(ELF) $(BIN) $(HEX) $(LST) size

$(LST) : $(ELF)
	@echo " OBJDUMP" $(notdir $@)
	@$(OBJDUMP) -x -S $< > $@

$(BIN) : $(ELF)
	@echo " OBJCOPY" $(notdir $@)
	@$(OBJCOPY) -O binary $< $@

$(HEX) : $(ELF)
	@echo " OBJCOPY" $(notdir $@)
	@$(OBJCOPY) -O ihex $< $@

$(ELF) : $(OBJS)
	@echo " LD" $@
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)
	
%.o : %.c
	@echo " CC" $(notdir $@)
	@$(CC) -c $(CFLAGS) $< -o $@

%.o : %.s
	@echo " AS" $(notdir $@)
	@$(AS) $(ASFLAGS) $< -o $@
	

size : $(ELF)
	@echo " SIZE" $(notdir $@)
	@$(SIZE) $<

clean :
	@echo " Clean binaries"
	@rm -f *.elf
	@rm -f *.bin
	@rm -f *.hex
	@rm -f *.lst
	@echo " Clean objs"
	@rm -f $(OBJS)


flash: $(BIN)
	@echo " Flashing..."
	@ST-LINK_CLI -NoPrompt -Q -c SWD FREQ=4000000 UR -P $(BIN) 0x08000000 -V while_programming -HardRst

.PHONY : clean size flash
