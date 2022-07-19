C_SOURCES = $(wildcard kern/*.c drivers/*/*.c  cpu/*.c libc/*.c)
ASM_SOURCES = $(wildcard drivers/*/*.asm)
HEADERS = $(wildcard kern/*.h drivers/*.h cpu/*.h libc/*.h)

# Nice syntax for file extension replacement

OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 
ASM = ${ASM_SOURCES:.asm=.o}

# Change this if your cross-compiler is somewhere else

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

# -g: Use debugging symbols in gcc

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -gdwarf-4 -Iinc/

# First rule is run by default

lunix.bin: boot/bootsect.bin kernel.bin
	cat $^ > lunix.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case

kernel.bin: boot/kernel_entry.o ${OBJ} ${ASM}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: lunix.bin
	qemu-system-i386 -fda lunix.bin --enable-kvm

# Open the connection to qemu and load our kernel-object file with symbols

debug: lunix.bin kernel.elf
	qemu-system-i386 -s -fda lunix.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o lunix.bin *.elf
	rm -rf kern/*.o boot/*.bin drivers/*/*.o boot/*.o cpu/*.o libc/*.o
