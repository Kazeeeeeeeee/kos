#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include "screen.h"

// 定義方向鍵的 Scancode (掃描碼 - Set 1)
#define KEY_UP    0x48
#define KEY_DOWN  0x50
#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

/* * poll_keyboard_and_move
 * 作用：檢查是否有按鍵按下，並根據方向鍵移動光標
 */
static inline void keyboard_move_cursor() {
    // 1. 檢查鍵盤狀態端口 (0x64)
    // 讀取狀態並使用位元遮罩 (& 0x01) 檢查最低位元是否為 1
    if (inb(0x64) & 0x01) {
        
        // 2. 有資料準備好！從資料端口 (0x60) 讀取 Scancode
        uint8 scancode = inb(0x60);

        // 3. 根據按下的按鍵，修改 screen.h 中的 vga_row 與 vga_col
        switch (scancode) {
            //case KEY_UP:
            //    if (vga_row > 0) vga_row--;
            //    break;
            //case KEY_DOWN:
            //    if (vga_row < vga_rows - 1) vga_row++;
            //    break;
            case KEY_LEFT:
                if (vga_col > 0){ 
			vga_col--;
		}else if(vga_row > 0){
			vga_col = vga_cols - 1;
			vga_row--;
		}
                break;
            case KEY_RIGHT:
                if (vga_col < vga_cols - 1){
			vga_col++;
		}else if(vga_row < vga_rows - 1){
			vga_col = 0;
			vga_row++;
		}
                break;
        }

        // 4. 更新硬體光標在畫面上的實際位置
        move_cursor(vga_row, vga_col);
    }
}

#endif // KEYBOARD_H
