
#include "include/screen.h"
#include "include/keyboard.h"

void kmain(){
	move_cursor(9,0);
	while(true)
	{
		keyboard_move_cursor();	
	}
	//put_char('A');
}
