C_SOURCES = $(wildcard kern/*.c drivers/*/*.c  arch/*/*.c libc/*.c)
ASM_SOURCES = $(wildcard drivers/*/*.asm arch/*/*.asm)
S_SOURCES = $(wildcard arch/*/*.s)
HEADERS = $(wildcard kern/*.h drivers/*.h arch/*/*.h libc/*.h)

# Nice syntax for file extension replacement

OBJ = ${C_SOURCES:.c=.o arch/i386/interrupt.o} 
ASM = ${ASM_SOURCES:.asm=.o}
SAS = ${S_SOURCES:.s=.o}

# Change this if your cross-compiler is somewhere else

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
AS = = /usr/local/i386elfgcc/bin/i386-elf-as

# -g: Use debugging symbols in gcc

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -gdwarf-4 -Iinc/

# First rule is run by default

lunix.img: boot/bootsect.bin lunix.bin
	cat $^ > lunix.img

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case

lunix.bin: arch/i386/kentry.o ${OBJ} ${ASM} ${SAS}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes

lunix.elf: arch/i386/kentry.o ${OBJ} ${ASM}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: lunix.bin
	qemu-system-i386 -fda lunix.img --enable-kvm

# Open the connection to qemu and load our kernel-object file with symbols

debug: lunix.bin kernel.elf
	qemu-system-i386 -s -fda lunix.img -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.o: %.s
	${CC} -c $< -o $@ 

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o lunix.bin *.elf
	rm -rf kern/*.o boot/*.bin drivers/*/*.o boot/*.o arch/*/*.o libc/*.o
