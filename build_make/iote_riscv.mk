################################################################################
# micro T-Kernel 3.00.07  makefile fragment for RISC-V
################################################################################

GCC := riscv64-unknown-elf-gcc
AS := riscv64-unknown-elf-gcc
LINK := riscv64-unknown-elf-gcc

# Select the target width and ABI at invocation time, for example:
#   make -C build_make RISCV_XLEN=64 RISCV_MABI=lp64
RISCV_XLEN ?= 32
RISCV_MARCH ?= rv$(RISCV_XLEN)ima_zicsr_zifencei_zicntr
RISCV_MABI ?= $(if $(filter 64,$(RISCV_XLEN)),lp64,ilp32)
RISCV_BOARD ?= simrv

ifeq ($(RISCV_BOARD),clint)
RISCV_BOARD := simrv
endif

ifeq ($(filter simrv rvcomp,$(RISCV_BOARD)),)
$(error RISCV_BOARD must be simrv or rvcomp)
endif
ifeq ($(RISCV_BOARD),simrv)
RISCV_BOARD_DEFS := -DRISCV_BOARD_SIMRV -DRISCV_TIMER_HZ=1000000U
endif
ifeq ($(RISCV_BOARD),rvcomp)
RISCV_BOARD_DEFS := -DRISCV_BOARD_RVCOMP -DRISCV_TIMER_HZ=150000000U
endif

ifeq ($(filter 32 64,$(RISCV_XLEN)),)
$(error RISCV_XLEN must be 32 or 64)
endif
ifeq ($(RISCV_XLEN),32)
ifneq ($(RISCV_MABI),ilp32)
$(error RV32 requires RISCV_MABI=ilp32)
endif
endif
ifeq ($(RISCV_XLEN),64)
ifneq ($(RISCV_MABI),lp64)
$(error RV64 requires RISCV_MABI=lp64)
endif
endif

RISCV_FLAGS := -march=$(RISCV_MARCH) -mabi=$(RISCV_MABI) -mcmodel=medany

CFLAGS := $(RISCV_FLAGS) $(RISCV_BOARD_DEFS) -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \

ASFLAGS := $(RISCV_FLAGS) $(RISCV_BOARD_DEFS) -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \

LFLAGS := $(RISCV_FLAGS) -ffreestanding \
    -nostartfiles -nostdlib \
    -O0 -g3 \

LNKFILE := "../etc/linker/iote_riscv/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/iote_riscv/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/riscv/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/iote_riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/riscv/subdir.mk
