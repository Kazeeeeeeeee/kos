#!/bin/bash

# 開啟「遇到錯誤立刻停止」的功能
set -e

echo "Compling ~w~......"
nasm -f elf32 kernel.asm -o kasm.o
gcc -m32 -c kernel.c -o kc.o -ffreestanding -O0 -fno-stack-protector

echo "loading linker.ld"
ld -m elf_i386 -n -T linker.ld -o kernel.bin kasm.o kc.o

# 檢查檔案是否真的存在
if [ ! -f "kernel.bin" ]; then
    echo "kernel.bin didn't generated succesfully"
    exit 1
fi
echo "kernel.bin success!!"

echo "3. 準備 ISO 目錄結構..."
rm -rf iso
mkdir -p iso/boot/grub

echo "copying kernel.bin into directory"
cp kernel.bin iso/boot/
cat <<EOF > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "KOS" {
    multiboot /boot/kernel.bin
    boot
}
EOF

echo "making .iso file......"
grub-mkrescue -o kos.iso iso
echo "kos.iso built!!!!"

echo "Welcome to KOS!!!!"
qemu-system-i386 -vga std -cdrom kos.iso -d int,guest_errors -no-reboot -no-shutdown
