
#include "include/screen.h"
#include "include/keyboard.h"

void kmain(){
	clear_screen();
	//move_cursor(0,0);
	print_string("Hello! Welcome to KOS! It is good to have you here. ^w^\n\n");
	//print_ascii_art();
	print_prompt();
	while(true)
	{
		//keyboard_move_cursor();
		poll_keyboard();	
	}
	//put_char('A');
}
