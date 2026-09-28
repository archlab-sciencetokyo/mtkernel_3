OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/int_riscv.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/wusec_riscv.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/int_riscv.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/wusec_riscv.d 

mtkernel_3/lib/libtk/sysdepend/cpu/core/riscv/%.o: ../lib/libtk/sysdepend/cpu/core/riscv/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
