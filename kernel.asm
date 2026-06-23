MAGIC	equ 0x1BADB002	;創建變量
MBALIGN	equ	1 << 0
MEMINFO equ	1 << 1
FLAGS	equ	MBALIGN | MEMINFO	;同時開啓MBALIGN以及MEMINFO
CHECKSUM	equ	-(MAGIC + FLAGS) ; 確保校驗結果為0，聲明内核為操作系統内核



section .multiboot
	align 4 ;將data對齊為4位元
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
	align 16
	stack_bottom:
		resb	4096 ;標準記憶體分頁（向上占用4096位）
	stack_top:;將頂部設置爲向上的4096位的頂樓
global start
extern kmain ;聲明函數在別處
section .text
	start:
		mov esp, stack_top ;將指針設置爲stacktop

		call kmain

	hang: ;當void kmain()結束了，確保中斷硬件，讓CPU休眠直到下一次重開機
		cli
		hlt
		jmp hang
