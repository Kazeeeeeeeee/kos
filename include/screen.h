#ifndef SCREEN_H
#define	SCREEN_H

#include "datatypes.h"
#include "io.h"

int8 vga_row = 9, vga_col = 0; ///<游標起始位置
const uint8 vga_cols = 80, vga_rows = 25, vga_dep = 2; ///<使用const將參數設置為只讀，屏幕大小，vga_dep的第一個byte儲存ascii編碼，后一個byte儲存屬性

string vga_buffer = (string)0xb8000; ///<VGA緩衝區域,跟dataype的定義，string其實就是記憶體指針

void put_char(char c){
	vga_buffer[0] = c;
	vga_buffer[1] = 0x0F;
}


static inline void move_cursor(uint8 row, uint8 cols){  ///<移動光標
	uint16 pos = (row * vga_cols) + cols; ///<計算1D陣列的位置 e.g 第一行第五列，位置就是1*80 + 5 = 85
	///< 0x0F VGA控制器游標控制器中“游標位置低位元組”暫存器編號
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8)((pos >> 8) & 0xFF));

}

#endif
