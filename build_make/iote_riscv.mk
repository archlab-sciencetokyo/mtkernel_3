################################################################################
# micro T-Kernel 3.00.07  makefile fragment for RISC-V
################################################################################

GCC := riscv64-unknown-elf-gcc
AS := riscv64-unknown-elf-gcc
LINK := riscv64-unknown-elf-gcc

CFLAGS := -march=rv32imac_zicsr -mabi=ilp32 -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \

ASFLAGS := -march=rv32imac_zicsr -mabi=ilp32 -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \

LFLAGS := -march=rv32imac_zicsr -mabi=ilp32 -ffreestanding \
    -nostartfiles \
    -O0 -g3 \

LNKFILE := "../etc/linker/iote_riscv/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/iote_riscv/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/riscv/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/iote_riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/riscv/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/riscv/subdir.mk
