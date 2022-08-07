C_SOURCES = $(wildcard *.c drivers/*/*.c ../libc/*.c arch/i386/*.c)
ASM_SOURCES = $(wildcard *.asm arch/i386/*.asm drivers/*/.asm)
S_SOURCES = $(wildcard *.s)
HEADERS = $(wildcard kern/*.h drivers/*.h arch/*/*.h libc/*.h)

# Nice syntax for file extension replacement

OBJ = ${C_SOURCES:.c=.o} 
ASM = ${ASM_SOURCES:.asm=.o}
SAS = ${S_SOURCES:.s=.o}

# Change this if your cross-compiler is somewhere else

CC = gcc
GDB = gdb
AS = as

# -g: Use debugging symbols in gcc

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -gdwarf-4 -I../inc/ -Wl,--build-id=none

lunix.krn: arch/i386/boot.o ${OBJ} ${ASM} ${SAS}
	ld -Tkern.ld -melf_i386 -o $@ $^ 

run: img
	qemu-system-i386 -cdrom lunix.iso --enable-kvm

# Open the connection to qemu and load our kernel-object file with symbols

img: lunix.krn
	mkdir lunix
	mkdir lunix/boot
	mkdir lunix/boot/grub
	cp lunix.krn lunix/boot/lunix.krn
	cp ../initrd/initrd.img lunix/boot/initrd
	echo "set timeout=10"                    >> lunix/boot/grub/grub.cfg
	echo "set default=0"                     >> lunix/boot/grub/grub.cfg
	echo ""                                  >> lunix/boot/grub/grub.cfg
	echo 'menuentry "Lunix OS" {'            >> lunix/boot/grub/grub.cfg
	echo "  multiboot /boot/lunix.krn"       >> lunix/boot/grub/grub.cfg
	echo "  module /boot/initrd"             >> lunix/boot/grub/grub.cfg
	echo "  boot"                            >> lunix/boot/grub/grub.cfg
	echo "}"                                 >> lunix/boot/grub/grub.cfg
	grub-mkrescue --output=lunix.iso lunix
	rm -rf lunix

# Generic rules for wildcards
# To make an object, always compile from its .c

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -o $@

%.o: %.s
	as --32 $< -o $@ 

%.bin: %.asm
	nasm $< -f elf32 -o $@

clean:
	rm -rf *.bin *.dis *.o lunix.krn *.elf *.img *.iso
	rm -rf kern/*.o drivers/*/*.o arch/*/*.o ../libc/*.o
