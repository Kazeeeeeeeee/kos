[bits 32]

; ------------------------------------------------
; 1. Multiboot 常數定義 (建議放在最上面)
; ------------------------------------------------
MAGIC equ 0x1BADB002
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
VBE equ 1 << 2
FLAGS equ MBALIGN | MEMINFO | VBE
CHECKSUM equ -(MAGIC + FLAGS)

; ------------------------------------------------
; 2. Multiboot 標頭區段
; ------------------------------------------------
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    
    ; 必須包含這些保留欄位，即便設為 0
    dd 0, 0, 0, 0, 0
    
    ; VBE 模式請求
    dd 0    ; Mode Type (0 = 線性圖形模式)
    dd 1024 ; Width
    dd 768  ; Height
    dd 32   ; Depth

; ------------------------------------------------
; 3. 程式碼區段 (入口點)
; ------------------------------------------------
section .text
global start
extern kmain

start:
    ; 【修正重點】：必須先設定好堆疊，才能 push 參數！
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

; ------------------------------------------------
; 4. 未初始化資料區段 (設定堆疊空間)
; ------------------------------------------------
section .bss
align 16
stack_bottom:
    resb 4096   ; 預留 4KB 作為堆疊
stack_top:
