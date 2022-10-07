CC = /local/apps/pulp/pulp-riscv-gnu-toolchain/bin/riscv32-unknown-elf-gcc
ifdef PULP_CC
	CC = $(PULP_CC)
endif
    

LIBRT = librt.a
LIBRTIO = librtio.a

include shared_flags.mk

CFLAGS += -Os -g
CFLAGS += -fno-jump-tables
CFLAGS += -Werror -Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wundef
CFLAGS += -fno-tree-loop-distribute-patterns

INC += -Iinclude/io
INC += -Iinclude
INC += -include include/fc_config.h

OBJ_DIR = obj
OUT_DIR = lib

LIBRT_OBJECTS += $(patsubst %.c,$(OBJ_DIR)/%.o,$(shell find pulp-rt/kernel -type f -name "*.c"))
LIBRT_OBJECTS += $(patsubst %.S,$(OBJ_DIR)/%.o,$(shell find pulp-rt/kernel -type f -name "*.S"))
LIBRT_OBJECTS += $(patsubst %.c,$(OBJ_DIR)/%.o,$(shell find pulp-rt/drivers_deprecated -type f -name "*.c"))
LIBRT_OBJECTS += $(patsubst %.S,$(OBJ_DIR)/%.o,$(shell find pulp-rt/drivers_deprecated -type f -name "*.S"))

LIBRTIO_OBJECTS += $(patsubst %.c,$(OBJ_DIR)/%.o,$(shell find pulp-rt/libs/io -type f -name "*.c"))

$(LIBRT): $(LIBRT_OBJECTS)
	mkdir -p $(OUT_DIR)
	ar -r -o $(OUT_DIR)/$@ $^

$(LIBRTIO): $(LIBRTIO_OBJECTS)
	mkdir -p $(OUT_DIR)
	ar -r -o $(OUT_DIR)/$@ $^

.SECONDEXPANSION:
$(OBJ_DIR)/%.o: %.c | $$(@D)/.
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: %.S | $$(@D)/.
	$(CC) $(CFLAGS) $(INC) -DLANGUAGE_ASSEMBLY -c $< -o $@

.PRECIOUS: $(OBJ_DIR)/. $(OBJ_DIR)%/.

$(OBJ_DIR)/.:
	mkdir -p $@

$(OBJ_DIR)%/.:
	mkdir -p $@

.PHONY: all clean

all: $(LIBRT) $(LIBRTIO)

clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)
