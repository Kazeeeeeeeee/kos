#ifndef SHELL_H
#define SHELL_H

#include "screen.h" // 需要存取 vga_row, vga_col 與 VGA 緩衝區

#define PROMPT "KOS> "
#define PROMPT_LEN 5

// 自定義字串比較 (等同於標準 C 語言的 strcmp)
static inline int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// 自定義部分字串比較 (給 echo 用，例如比對前 5 個字元 "echo ")
static inline int strncmp(const char *s1, const char *s2, int n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// 實作 clear 指令：清空螢幕
static inline void clear_screen() {
    uint16 *vga_buffer = (uint16 *)0xB8000;
    for (int i = 0; i < vga_cols * vga_rows; i++) {
        vga_buffer[i] = (0x0F << 8) | ' '; // 填滿黑色背景與空白字元
    }
    vga_row = 0;
    vga_col = 0;
}


// 印出提示字元
static inline void print_prompt() {
    print_string(PROMPT);
    move_cursor(vga_row, vga_col);
}

// 核心命令處理器
static inline void execute_command(char *cmd) {
    if (cmd[0] == '\0') {
        return; // 什麼都沒打直接按 Enter
    }

    if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "help") == 0) {
        print_string("Commands available:\n");
        print_string("  clear - Clear the terminal screen\n");
        print_string("  echo  - Print text (e.g., echo hello)\n");
        print_string("  help  - Show this help message\n");
        print_string("  info  - Show OS version information\n");
    } else if (strcmp(cmd, "info") == 0) {
        print_string("KOS v0.1\n");
        print_string("Kernel loaded successfully!\n");
    } else if (strcmp(cmd, "echo") == 0 || strncmp(cmd, "echo ", 5) == 0) {
        // 從第 5 個字元開始印 (跳過 "echo ")
        if(strncmp(cmd, "echo ", 5) == 0){	
	    print_string(cmd + 5);	
	    print_string("\n");
    	}else{
	    print_string("This is a string output function, try type <echo Hello World!> \n");
	}	    
    } else if(strcmp(cmd,"miku") == 0){
    	print_miku();
    	print_string("\n");
    }else {
        print_string("Command not found: ");
        print_string(cmd);
        print_string("\n");
    }
}

#endif 
