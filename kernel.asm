[bits 32]

MAGIC equ 0x1BADB002
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
VBE equ 1 << 2
FLAGS equ MBALIGN | MEMINFO | VBE
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    
    dd 0, 0, 0, 0, 0
    
    ; VBE 模式請求
    dd 0    ; Mode Type (0 = 線性圖形模式)
    dd 1024 ; Width
    dd 768  ; Height
    dd 32   ; Depth

section .text
global start
extern kmain

start:
    mov esp, stack_top
    
    ; 傳遞 MBI 指標與 Magic Number 給 kmain
    ; C 語言呼叫慣例 (cdecl) 參數從右到左推入：kmain(magic, mbi)
    push ebx    ; 推入第二個參數 (mbi 指標)
    push eax    ; 推入第一個參數 (magic number)
    
    call kmain

hang:
    cli
    hlt
    jmp hang

; 設定堆疊空間
section .bss
align 16
stack_bottom:
    resb 4096   ; 預留 4KB 作為堆疊
stack_top:
