#ifndef SCREEN_H
#define	SCREEN_H

#include "datatypes.h"

int8 vga_row = 0, vga_col = 0; ///<游標起始位置
const uint8 vga_cols = 80, vga_rows = 25, vga_dep = 2; ///<使用const將參數設置為只讀，屏幕大小，vga_dep的第一個byte儲存ascii編碼，后一個byte儲存屬性

string vga_buffer = (string)0xb8000; ///<VGA緩衝區域,跟dataype的定義，string其實就是記憶體指針

void put_char(char c){
	vga_buffer[0] = c;
	vga_buffer[1] = 0x0F;
}

#endif
