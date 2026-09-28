OUTPUT_ARCH(riscv)
ENTRY(_start)

/* Memories definition */
MEMORY
{
  ROM (rx)		: ORIGIN = 0x80000000, LENGTH = 128K
  RAM (xrw)		: ORIGIN = 0x80020000, LENGTH = 128K
}

SECTIONS {
	.text : {
		. = ALIGN(4);
		__start = .;
		KEEP(*(.text.startup))
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
		__text_end = .;
	} >ROM

	. = ALIGN(4);

	__data_org = LOADADDR(.data);
	.data : {
		PROVIDE(__global_pointer$ = . + 0x800);
		__data_start = .;
		*(.data)
		*(.data.*)
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(4);
		__data_end = .;
	} >RAM AT >ROM
	
	.bss ALIGN(4) (NOLOAD) : {
		__bss_start = .;
		PROVIDE (__noinit_start = .);
		*(.noinit)
		PROVIDE (__noinit_end = .);
		*(.bss)
		*(.bss.*)
		*(.sbss)
		*(.sbss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
		__end = .;
	} >RAM

	/* Define the top of stack */
	_stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
