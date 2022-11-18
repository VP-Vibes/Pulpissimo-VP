APP = i2s

OUT_DIR = build
OBJ_DIR = $(OUT_DIR)/obj

CC = $(RISCV)/bin/riscv32-unknown-elf-gcc

include $(PULP_SDK_HOME)/shared_flags.mk
CFLAGS += -O3 -g

INC += -Iincludes
INC += -I$(PULP_SDK_HOME)/include
INC += -I$(PULP_SDK_HOME)/include/io
INC += -include $(PULP_SDK_HOME)/include/fc_config.h

LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections
LDFLAGS += -L$(PULP_SDK_HOME)/pulp-rt/rules
LDFLAGS += -Tlink.ld
LDFLAGS += -L$(PULP_SDK_HOME)/lib
LDFLAGS += -lrt -lrtio -lrt -lgcc

CONF = $(PULP_SDK_HOME)/pulp-rt/conf
OBJECTS += $(patsubst %.c,$(OBJ_DIR)/%.o,$(shell find . -type f -name "*.c"))
OBJECTS += $(patsubst %.c,$(OBJ_DIR)/%.o,$(shell find $(CONF) -type f -name "*.c" -exec basename \{\} \;))

$(OUT_DIR)/$(APP): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(CONF)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PHONY: all clean dirmake

$(APP).slm: dirmake $(OUT_DIR)/$(APP)
	$(PULP_SDK_HOME)/pulp-tools/plp_mkflash --flash-boot-binary=$(OUT_DIR)/$(APP) --stimuli=$@ --flash-type=spi --qpi --archi=pulpissimo

$(APP).stim: dirmake $(OUT_DIR)/$(APP)
	$(PULP_SDK_HOME)/pulp-tools/stim_utils.py --binary=$(OUT_DIR)/$(APP) --vectors=$@

dirmake:
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)

all: $(APP).slm $(APP).stim

clean:
	rm -rf $(APP).slm
	rm -rf $(APP).stim
	rm -rf $(OUT_DIR)
