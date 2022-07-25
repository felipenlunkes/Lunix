C_SOURCES = $(wildcard kern/*.c drivers/*/*.c  arch/*/*.c libc/*.c)
ASM_SOURCES = $(wildcard drivers/*/*.asm arch/*/*.asm)
S_SOURCES = $(wildcard arch/*/*.s)
HEADERS = $(wildcard kern/*.h drivers/*.h arch/*/*.h libc/*.h)

# Nice syntax for file extension replacement

OBJ = ${C_SOURCES:.c=.o arch/i386/interrupt.o} 
ASM = ${ASM_SOURCES:.asm=.o}
SAS = ${S_SOURCES:.s=.o}

# Change this if your cross-compiler is somewhere else

CC = /usr/local/lunixgcc/bin/i386-elf-gcc
GDB = /usr/local/lunixgcc/bin/i386-elf-gdb
AS = = /usr/local/lunixgcc/bin/i386-elf-as

# -g: Use debugging symbols in gcc

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -gdwarf-4 -Iinc/ -Wl,--build-id=none

lunix.bin: boot/LXboot.o ${OBJ} ${ASM} ${SAS}
	i386-elf-ld -T kern/kern.ld -melf_i386 -o $@ $^ 

run: img 
	qemu-system-i386 -cdrom lunix.iso --enable-kvm

# Open the connection to qemu and load our kernel-object file with symbols

img: lunix.bin
	mkdir lunix
	mkdir lunix/boot
	mkdir lunix/boot/grub
	cp lunix.bin lunix/boot/lunix.bin
	echo "set timeout=10"                    >> lunix/boot/grub/grub.cfg
	echo "set default=0"                     >> lunix/boot/grub/grub.cfg
	echo ""                                  >> lunix/boot/grub/grub.cfg
	echo 'menuentry "Lunix OS" {'            >> lunix/boot/grub/grub.cfg
	echo "  multiboot /boot/lunix.bin"       >> lunix/boot/grub/grub.cfg
	echo "  boot"                            >> lunix/boot/grub/grub.cfg
	echo "}"                                 >> lunix/boot/grub/grub.cfg
	grub-mkrescue --output=lunix.iso lunix
	rm -rf lunix

debug: lunix.bin kernel.elf
	qemu-system-i386 -s -fda lunix.img -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

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
	rm -rf *.bin *.dis *.o lunix.bin *.elf *.img *.iso
	rm -rf kern/*.o boot/*.bin drivers/*/*.o boot/*.o arch/*/*.o libc/*.o
