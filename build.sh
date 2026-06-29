#!/bin/bash

# 開啟「遇到錯誤立刻停止」的功能
set -e

echo "1. 開始編譯核心 (NASM & GCC)..."
nasm -f elf32 kernel.asm -o kasm.o
gcc -m32 -c kernel.c -o kc.o -ffreestanding -O0 -fno-stack-protector

echo "2. 開始連結核心 (LD)..."
ld -m elf_i386 -n -T linker.ld -o kernel.bin kasm.o kc.o

# 檢查檔案是否真的存在
if [ ! -f "kernel.bin" ]; then
    echo "❌ 嚴重錯誤：kernel.bin 沒有被成功生成！"
    exit 1
fi
echo "✅ kernel.bin 編譯與連結成功！"

echo "3. 準備 ISO 目錄結構..."
rm -rf iso
mkdir -p iso/boot/grub

echo "4. 複製核心與生成 GRUB 設定檔..."
cp kernel.bin iso/boot/
cat <<EOF > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "KOS" {
    multiboot /boot/kernel.bin
    boot
}
EOF

echo "5. 打包成 kos.iso..."
grub-mkrescue -o kos.iso iso
echo "✅ ISO 建立成功！準備啟動 QEMU..."

echo "6. 啟動 QEMU..."
qemu-system-i386 -vga std -cdrom kos.iso -d int,guest_errors -no-reboot -no-shutdown
