ENTRY(main)
SECTIONS {
	  . = 0x82000000;
	  .text : { *(.text*) }
	  .rodata : { *(.rodata*) }
	  .data : { *(.data*) }
	  .bss : { *(.bss*) }
}
