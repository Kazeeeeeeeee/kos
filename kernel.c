#include "include/screen.h"
#include "include/multiboot.h"

void kmain(uint32 magic, multiboot_info_t *mbi) {
    // Multiboot 規範中，flags 的第 12 位元 (0x1000) 若為 1，表示 framebuffer 有效
    if (!(mbi->flags & (1 << 12))) {
        // 若沒有圖形資訊，這裡死循環避免存取錯誤位址
        while (1); 
    }
    // 從 mbi 取得正確的線性幀緩衝區位址
    framebuffer = (uint32 *)(uint32)mbi->framebuffer_addr;

    // 初始化
    clear_screen(0x00000000);
    // square drawing test for frame buffer
    //for (int y = 200; y < 300; y++) {
    //    for (int x = 200; x < 300; x++) {
    //        put_pixel(x, y, 0xFFFFFF);
    //    }
    //}

    //draw_char(10, 10, 'A', 0xFFFFFF);
    print_string(10, 10, "Hello, KOS!", 0xFFFFFF);
    while (1) {
    }
}
