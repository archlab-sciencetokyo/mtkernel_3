S_UPPER_SRCS += \
../mtkernel_3/kernel/sysdepend/cpu/core/riscv/dispatch.S \
../mtkernel_3/kernel/sysdepend/cpu/core/riscv/startup.S 

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/cpu_cntl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/dispatch.o \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/exc_hdr.o \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/interrupt.o \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/reset_hdl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/startup.o 

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/dispatch.d \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/startup.d 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/cpu_cntl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/exc_hdr.d \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/interrupt.d \
./mtkernel_3/kernel/sysdepend/cpu/core/riscv/reset_hdl.d 

mtkernel_3/kernel/sysdepend/cpu/core/riscv/%.o: ../kernel/sysdepend/cpu/core/riscv/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/core/riscv/%.o: ../kernel/sysdepend/cpu/core/riscv/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
