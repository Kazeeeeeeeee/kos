ENTRY(start)

SECTIONS
{
    /* 作業系統核心通常載入在 1MB 處 */
    . = 1M;

    /* 強制將 .multiboot 段落放在檔案最開頭，且保證 4K 對齊 */
    .multiboot ALIGN(4K) :
    {
        *(.multiboot)
    }

    .text ALIGN(4K) :
    {
        *(.text)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(.bss)
        *(COMMON)
    }
}
