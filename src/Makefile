ENABLE_DEBUG ?=

ifneq ($(V), 1)
MAKEFLAGS += --no-print-dir
Q := @
endif

OPT_FLAGS ?= -O2

CFLAGS += -Wall -Wextra -Werror -Wno-char-subscripts\
	$(OPT_FLAGS) -std=gnu99 -g3 -MD \
	-I.
LDFLAGS += $(OPT_FLAGS)

ifeq ($(ENABLE_DEBUG), 1)
CFLAGS += -DENABLE_DEBUG
endif

SRC =			\
	startup.c main.c

CROSS_COMPILE ?= riscv-sifive-elf-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

OPT_FLAGS = -Os
CFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medany \
	-DSTM32F1 -DDISCOVERY_STLINK -I../libopencm3/include \
	-I .
LDFLAGS_BOOT := $(LDFLAGS) --specs=nano.specs \
	-lopencm3_stm32f1 -Wl,--defsym,_stack=0x20005000 \
	-Wl,-T,redpill.ld -nostartfiles -lc \
	-Wl,-Map=mapfile -march=rv32imac -mabi=ilp32 -mcmodel=medany -Wl,-gc-sections \
	-L../libopencm3/lib
LDFLAGS = $(LDFLAGS_BOOT)

ifeq ($(ENABLE_DEBUG), 1)
LDFLAGS += --specs=rdimon.specs
else
LDFLAGS += --specs=nosys.specs
endif

all:	pill_serial.bin

host_clean:
	-$(Q)$(RM) pill_serial.bin

OBJ = $(SRC:.c=.o)

pill_serial.elf: $(OBJ)
	@echo "  LD      $@"
	$(Q)$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o:	%.c
	@echo "  CC      $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

%.bin:	%.elf
	@echo "  OBJCOPY $@"
	$(Q)$(OBJCOPY) -O binary $^ $@

%.hex:	%
	@echo "  OBJCOPY $@"
	$(Q)$(OBJCOPY) -O ihex $^ $@

.PHONY:	clean host_clean FORCE

clean:	host_clean
	$(Q)echo "  CLEAN"
	-$(Q)$(RM) -f *.o *.d *~ *.elf *.bin $(HOSTFILES)
	-$(Q)$(RM) -f mapfile

-include *.d

flash: pill_serial.bin
	dfu-util -a 0 -d 28e9:0189 -s 0x08000000:leave -D pill_serial.bin
