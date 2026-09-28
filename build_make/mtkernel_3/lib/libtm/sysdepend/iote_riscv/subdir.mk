OBJS += \
./mtkernel_3/lib/libtm/sysdepend/iote_riscv/tm_com.o 

C_DEPS += \
./mtkernel_3/lib/libtm/sysdepend/iote_riscv/tm_com.d 

mtkernel_3/lib/libtm/sysdepend/iote_riscv/%.o: ../lib/libtm/sysdepend/iote_riscv/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
