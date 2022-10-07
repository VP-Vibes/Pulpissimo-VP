#ifndef __PULP_FC_CONFIG_H__
#define __PULP_FC_CONFIG_H__

#include "archi/pulp_defs.h"

// PULP builder defines from Makefile
#define __riscv__ 1
#define __RT_USE_IO 1
#define __RT_USE_ASSERT 1
#define __RT_USE_BRIDGE 1
#define __RT_USE_WARNING 1
#define CONFIG_CHECK_CLUSTER_START 1
#define CONFIG_PADS_ENABLED 1
#define __RT_UDMA_COPY_ASM 1
#define __RT_I2C_COPY_ASM 1
#define __RT_SPIM_COPY_ASM 1
#define RT_CONFIG_RTC_ENABLED
#define RT_CONFIG_GPIO_ENABLED

// PULP builder defines
#define ARCHI_HAS_FC 1
#define PLP_HAS_FC 1
#define PLP_NO_BUILTIN 1
#define __RT_ALLOC_L2_MULTI 1
#define __BENCH__ 1
#define BENCH 1
#define PULP_RT_VERSION_RELEASE 0
#define PULP_RT_VERSION_BENCH 1
#define PULP_RT_VERSION_PROFILE0 2
#define PULP_RT_VERSION_PROFILE1 3
#define PULP_RT_VERSION_DEBUG 4
#define PULP_RT_VERSION PULP_RT_VERSION_BENCH
#define __PULP_OS__
#define __PULPOS__
#define PULP
#define __PULP__
#define __ri5cy__ 1
#define PULP_CHIP CHIP_PULPISSIMO
#define CONFIG_PULPISSIMO 1
#define PULP_CHIP_STR pulpissimo
#define PULP_CHIP_FAMILY CHIP_PULPISSIMO
#define PULP_CHIP_FAMILY_STR pulpissimo
#define PULP_CHIP_VERSION 0
#define PULP_CORE CORE_RISCV_V4
#define FP_SW_EMUL 1
#define ARCHI_CORE_HAS_1_10 1
#define PLP_NO_PERF_COUNTERS 1
#if defined(USE_PULP_EXTENSIONS) && USE_PULP_EXTENSIONS == 1
#define ARCHI_CORE_HAS_PULPV2 1
#else
#define RV_ISA_RV32 1
#endif

#endif
