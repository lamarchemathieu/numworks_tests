



NAME ?= output

FULLNAME = $(NAME)

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

SRCS =	$(wildcard source/*.c) \
		../../STM32Cube_FW_F4_V1.21.0/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
		../../STM32Cube_FW_F4_V1.21.0/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
		../../STM32Cube_FW_F4_V1.21.0/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c

SRCS_ASM = ../../STM32Cube_FW_F4_V1.21.0/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f412vx.s

		
INCLUDES =	-I./include \
			-I../../STM32Cube_FW_F4_V1.21.0/Drivers/CMSIS/Include \
			-I../../STM32Cube_FW_F4_V1.21.0/Drivers/CMSIS//Device/ST/STM32F4xx/Include \
			-I../../STM32Cube_FW_F4_V1.21.0/Drivers/STM32F4xx_HAL_Driver/Inc 

OBJS = $(SRCS:.c=.o) $(SRCS_ASM:.s=.o)

CFLAGS = $(INCLUDES)  -std=c99 -g -ffunction-sections -Os -mthumb -march=armv7e-m -mfloat-abi=softfp -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -DSTM32F412Vx -DUSE_FULL_LL_DRIVER
ASFLAGS = $(INCLUDES) -g -mthumb -march=armv7e-m -mfloat-abi=softfp -mcpu=cortex-m4 -mfpu=fpv4-sp-d16
LDSCRIPT = linker.ld
LDFLAGS = -T$(LDSCRIPT) -Wl,-gc-sections -mthumb -mcpu=cortex-m4

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

flash : $(BIN)
	@dfu-util -i 0 -a 0 -s 0x08000000 -D $<

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

.PHONY : clean size flash
