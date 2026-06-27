#ifndef IO_H
#define IO_H

/** 建立outb (Out Byte)函數
 * 作用：發送一字節的資料到指定的二字節記憶體位置(硬件溝通基石)
 * 參數：
 * port：硬體端口號碼（例如VGA命令端口0x3D4）
 * data：要發送的資料數值
 */

static inline void outb(unsigned short port, unsigned char data) {
    /* * 這裡使用了 GCC 的內聯組合語言 (Inline Assembly)。
     * "outb %0, %1" 是實際執行的組合語言指令。
     * "a"(data) 表示將 data 放入 AL 暫存器。
     * "Nd"(port) 表示將 port 放入 DX 暫存器。
     */
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

/* * inb (In Byte)
 * 作用：從指定的 16 位元硬體端口讀取 8 位元 (1 Byte) 的資料
 * 參數：
 * port: 硬體端口號碼 (例如鍵盤資料端口 0x60)
 * 回傳：讀取到的數值
 */
static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    /*
     * "=a"(result) 表示將 AL 暫存器的結果存入 result 變數。
     * "Nd"(port) 表示將 port 放入 DX 暫存器。
     */
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

#endif
