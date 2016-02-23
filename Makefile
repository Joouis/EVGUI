PROJ_NAME = stm32f429-example

##########################################
# Toolchain Settings
##########################################

CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

##########################################
# Files 
##########################################

APPS_SRCS = $(wildcard apps/*.c)
BACKEND_SRCS = $(wildcard backend/*.c)
STDLIB_SRCS = $(wildcard stdlib/src/*.c)
STDIO_SRCS = $(wildcard stdio/*.c)
STARTUP_SRCS = $(wildcard cmsis_boot/*.c)
SYSCALL_SRCS = $(wildcard syscalls/*.c)
UGUI_SRCS = $(wildcard uGUI/*c)
TWIN_SRCS = $(wildcard twin/src/*c)

OBJS = $(APPS_SRCS:.c=.o) \
		$(BACKEND_SRCS:.c=.o) \
		$(STDLIB_SRCS:.c=.o) \
		$(STDIO_SRCS:.c=.o) \
		$(STARTUP_SRCS:.c=.o) \
		$(SYSCALL_SRCS:.c=.o) \
		$(UGUI_SRCS:.c=.o) \
		$(TWIN_SRCS:.c=.o)

INCLUDES = -Icmsis \
			-Icmsis_boot \
			-Iapps \
			-Ibackend \
			-Istdlib/inc \
			-IuGUI \
			-Itwin/inc
##########################################
# Flag Settings 
##########################################

MCU = -mcpu=cortex-m4 -mthumb -march=armv7e-m -mtune=cortex-m4
FPU = -mfpu=fpv4-sp-d16 -mfloat-abi=hard -D__FPU_USED
DEFINES = -DSTM32F4XX -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -D__ASSEMBLY__

CFLAGS = $(MCU) $(FPU) $(DEFINES) $(INCLUDES) -g2 -Wall -O0 -c
LDFLAGS =
define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a) 
LDFLAGS += $(MCU) -g2 -nostartfiles \
	-Wl,-Map=$(PROJ_NAME).map -O0 -Wl,--gc-sections -Tld/stm32f4.ld

##########################################
# Targets
###########################################

all: $(PROJ_NAME).bin info

$(PROJ_NAME).elf: $(OBJS)
			 @$(CC) $(OBJS) $(LDFLAGS) -o $@
			 @echo $@

$(PROJ_NAME).bin: $(PROJ_NAME).elf
			 @$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
			 @echo $@

info: $(PROJ_NAME).elf
	@$(SIZE) --format=berkeley $(PROJ_NAME).elf

.c.o:
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

OPENOCD_INS = "flash write_image erase stm32f429-example.bin 0x8000000"

flash: $(PROJ_NAME.bin)
	openocd \
		-f interface/stlink-v2.cfg \
		-f target/stm32f4x.cfg \
		-c "init" \
		-c "reset init" \
		-c "flash probe 0" \
		-c "flash info 0" \
		-c $(OPENOCD_INS) \
		-c "reset run" -c shutdown 

debug: $(PROJ_NAME).elf
	arm-none-eabi-gdb -x "gdb.script"

clean:
	rm -f $(OBJS)
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map

