<p align='center'>
<a href="https://github.com/lunix"><img height="150" src="https://github.com/felipenlunkes/Lunix/blob/main/Doc/header.gif"></a>&nbsp;&nbsp;
</p>

# Lunix OS

Lunix is ​​a new operating system written in C for the x86 architecture for educational purposes, serving as a testing ground for my learning in C language, hardware and operating system architecture. Some components are written in x86 Assembly.

The system already supports:

* 32-bit protected mode;
* Keyboard, console, serial and parallel ports drivers and the list will only grow;
* Task and processes models;

To build this system, I've been applying my knowledge gained in [Hexagonix](https://github.com/hexagonix) development, taking advantage of assembly code or converting it to C.

The rudimentary foundation of Lunix OS was based on the great tutorials available [here](https://github.com/cfenollosa/os-tutorial). The system already has thread support and will soon be much more complete!

## Build

To build Lunix, you must build the cross-compiler tools. For that, you first need to install the dependencies to build gcc, binutils and the system:

```
sudo apt install gcc libmpc-dev nasm fasm
```

You will need to build binutils and a cross-compiled gcc, and put them into `/usr/local/lunixgcc`. To do that, use:

```
export PREFIX="/usr/local/lunixgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"
```

### Building binutils

To build binutils, use the follow shell commands:

```sh
mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.gz # If the link 404's, look for a more recent version
tar xf binutils-2.24.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.24/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
make all install 2>&1 | tee make.log
```

### Buildind GCC

Use the commands below:

```sh
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-4.9.1/gcc-4.9.1.tar.bz2
tar xf gcc-4.9.1.tar.bz2
mkdir gcc-build
cd gcc-build
../gcc-4.9.1/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
make all-gcc 
make all-target-libgcc 
make install-gcc 
make install-target-libgcc 
```

That's it! You should have all the GNU binutils and the compiler at `/usr/local/lunixgcc/bin`, prefixed by `i386-elf-lunix-` to avoid collisions with your system's compiler and binutils.

You may want to add the `$PATH` to your `.bashrc`.

### Build the system

To build the Lunix, run make on the source tree folder.

```
make
```

To test the system, use:

```
make run
```

