OBJS += \
./mtkernel_3/kernel/sysdepend/iote_riscv/cpu_clock.o \
./mtkernel_3/kernel/sysdepend/iote_riscv/devinit.o \
./mtkernel_3/kernel/sysdepend/iote_riscv/hw_setting.o \
./mtkernel_3/kernel/sysdepend/iote_riscv/power_save.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/iote_riscv/cpu_clock.d \
./mtkernel_3/kernel/sysdepend/iote_riscv/devinit.d \
./mtkernel_3/kernel/sysdepend/iote_riscv/hw_setting.d \
./mtkernel_3/kernel/sysdepend/iote_riscv/power_save.d 

mtkernel_3/kernel/sysdepend/iote_riscv/%.o: ../kernel/sysdepend/iote_riscv/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
