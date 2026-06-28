#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include "screen.h"
#include "shell.h" 

#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

const char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};


static inline void keyboard_move_cursor(uint8 scancode) {
    switch (scancode) {
        case KEY_LEFT:
            if (vga_col > PROMPT_LEN) {
                vga_col--;
            }
            break;
        case KEY_RIGHT:
            if (vga_col < vga_cols - 1) {
                vga_col++;
            }
            break;
    }
    move_cursor(vga_row, vga_col);
}

static inline void keyboard_handle_typing(uint8 scancode) {
    if (scancode < 128 && kbd_us[scancode] != 0) {
        char ascii = kbd_us[scancode];
        uint16 *vga_buffer = (uint16 *)0xB8000;
        
        if (ascii == '\n') { 
            // 從螢幕讀取指令
            char cmd_buffer[80];
            int idx = 0;
            int last_char_idx = -1;
            
            // 找出該行最後一個非空白字元的位置
            for (int i = PROMPT_LEN; i < vga_cols; i++) {
                char c = vga_buffer[vga_row * vga_cols + i] & 0xFF;
                if (c != ' ' && c != 0) last_char_idx = i;
            }
            
            // 將螢幕上的字元複製到 cmd_buffer
            if (last_char_idx >= PROMPT_LEN) {
                for (int i = PROMPT_LEN; i <= last_char_idx; i++) {
                    cmd_buffer[idx++] = vga_buffer[vga_row * vga_cols + i] & 0xFF;
                }
            }
            cmd_buffer[idx] = '\0'; // 字串結尾
            
            // 視覺換行
            vga_col = 0;
            if (vga_row < vga_rows - 1) vga_row++;
            
            // Shell 執行
            execute_command(cmd_buffer);
            
            // 印出新的提示字元
            print_prompt();

        } else if (ascii == '\b') { 
            // 處理 Backspace 退格,保護邊界為 PROMPT_LEN
            if (vga_col > PROMPT_LEN) {
                vga_col--;
                for (int i = vga_col; i < vga_cols - 1; i++) {
                    uint16 pos_curr = (vga_row * vga_cols) + i;
                    uint16 pos_next = (vga_row * vga_cols) + i + 1;
                    vga_buffer[pos_curr] = vga_buffer[pos_next];
                }
                uint16 pos_end = (vga_row * vga_cols) + vga_cols - 1;
                vga_buffer[pos_end] = (0x0F << 8) | ' ';
            }
        } else {
            // 一般打字,防止超出單行寬度
            if (vga_col < vga_cols - 1) {
                uint16 pos = (vga_row * vga_cols) + vga_col;
                vga_buffer[pos] = (0x0F << 8) | ascii;
                vga_col++;
            }
        }
        move_cursor(vga_row, vga_col);
    }
}

static inline void poll_keyboard() {
    if (inb(0x64) & 0x01) {
        uint8 scancode = inb(0x60);
        if (scancode < 0x80) {
            if (scancode == KEY_LEFT || scancode == KEY_RIGHT) {
                keyboard_move_cursor(scancode);
            } else {
                keyboard_handle_typing(scancode);
            }
        }
    }
}

#endif
