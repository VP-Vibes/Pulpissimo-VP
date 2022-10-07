# Shared flags are also used by executables (e.g., bootloader, apps)
#
# To enable pulp extensions change CFLAGS to:
# 		-march=rv32imcXpulpv2
# 		-DUSE_PULP_EXTENSIONS=1
CFLAGS += -march=rv32imc
CFLAGS += -DUSE_PULP_EXTENSIONS=0
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -MMD -MP
