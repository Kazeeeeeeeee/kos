#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include "screen.h"

// 定義方向鍵的 Scancode (掃描碼 - Set 1)
// 根據需求移除了上下鍵，僅保留左右橫向移動
#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

// 建立一個簡單的 Scancode 到 ASCII 轉換表 (對應美式 QWERTY 鍵盤的按下編碼)
// 陣列的索引 (Index) 剛好對應硬體傳來的 Scancode
const char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

/* * keyboard_move_cursor
 * 作用：處理左右方向鍵的光標移動與換行
 */
static inline void keyboard_move_cursor(uint8 scancode) {
    switch (scancode) {
        case KEY_LEFT:
            if (vga_col > 0) {
                vga_col--;
            } else if (vga_row > 0) {
                // 若在行首按下左鍵，退回上一行的最後一個字元位置
                vga_col = vga_cols - 1;
                vga_row--;
            }
            break;
        case KEY_RIGHT:
            if (vga_col < vga_cols - 1) {
                vga_col++;
            } else if (vga_row < vga_rows - 1) {
                // 當光標移動到窗口右邊緣時，自動換行到下一行
                vga_col = 0;
                vga_row++;
            }
            break;
    }
    // 更新硬體光標
    move_cursor(vga_row, vga_col);
}

/* * keyboard_handle_typing
 * 作用：處理文字輸入、換行 (Enter) 與退格 (Backspace)
 */
static inline void keyboard_handle_typing(uint8 scancode) {
    if (scancode < 128 && kbd_us[scancode] != 0) {
        char ascii = kbd_us[scancode];
        uint16 *vga_buffer = (uint16 *)0xB8000;
        
        if (ascii == '\n') { 
            vga_col = 0;
            if (vga_row < vga_rows - 1) vga_row++;
        } else if (ascii == '\b') { 
            // 【處理 Backspace 退格並自動合併】
            if (vga_col > 0) {
                // 1. 光標先退一格
                vga_col--;
                
                // 2. 將光標之後的所有字元往前移動一格
                // 我們從當前光標位置開始，把後面的內容複製過來，直到該行結尾
                for (int i = vga_col; i < vga_cols - 1; i++) {
                    uint16 pos_curr = (vga_row * vga_cols) + i;
                    uint16 pos_next = (vga_row * vga_cols) + i + 1;
                    vga_buffer[pos_curr] = vga_buffer[pos_next];
                }
                
                // 3. 清空最後一格 (確保沒有殘影)
                uint16 pos_end = (vga_row * vga_cols) + vga_cols - 1;
                vga_buffer[pos_end] = (0x0F << 8) | ' ';
                
            } else if (vga_row > 0) {
                // 若在行首，則退回上一行結尾（這部分保持不變或可進階優化）
                vga_col = vga_cols - 1;
                vga_row--;
            }
        } else {
            // 【處理一般文字打字】
            uint16 pos = (vga_row * vga_cols) + vga_col;
            vga_buffer[pos] = (0x0F << 8) | ascii;

            vga_col++;
            if (vga_col >= vga_cols) {
                vga_col = 0;
                if (vga_row < vga_rows - 1) vga_row++;
            }
        }
        move_cursor(vga_row, vga_col);
    }
}
/* * poll_keyboard
 * 作用：輪詢鍵盤狀態，並將接收到的掃描碼分發給對應的處理函數
 */
static inline void poll_keyboard() {
    // 1. 檢查鍵盤狀態端口 (0x64)
    // 讀取狀態並使用位元遮罩 (& 0x01) 檢查最低位元是否為 1
    if (inb(0x64) & 0x01) {
        
        // 2. 有資料準備好！從資料端口 (0x60) 讀取 Scancode
        uint8 scancode = inb(0x60);

        // 3. 過濾掉按鍵釋放 (Break) 的訊號 (Scancode >= 0x80)
        if (scancode < 0x80) {
            
            // 4. 判斷是方向鍵還是文字輸入，並進行分發 (Dispatch)
            if (scancode == KEY_LEFT || scancode == KEY_RIGHT) {
                keyboard_move_cursor(scancode);
            } else {
                keyboard_handle_typing(scancode);
            }
        }
    }
}

#endif // KEYBOARD_H
