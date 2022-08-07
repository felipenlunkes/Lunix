<p align='center'>
<a href="https://github.com/felipenlunkes/lunix"><img height="150" src="https://github.com/felipenlunkes/Lunix/blob/main/Doc/header.gif"></a>&nbsp;&nbsp;
</p>

# Lunix OS

<p align='center'>
<a href="https://github.com/felipenlunkes/lunix"><img height="440" src="https://github.com/felipenlunkes/Lunix/blob/main/Doc/Lunix-current.png"></a>&nbsp;&nbsp;
</p>

Lunix is a new operating system written in C for the x86 architecture for educational purposes, serving as a testing ground for my learning in C language, hardware and operating system architecture. Some components are written in x86 Assembly.

The system already supports:

* 32-bit protected mode;
* Keyboard, console, serial and parallel ports drivers and the list will only grow;
* Task and processes models;

To build this system, I've been applying my knowledge gained in [Hexagonix](https://github.com/hexagonix) development, taking advantage of assembly code or converting it to C.

The rudimentary foundation of Lunix OS was based on the great tutorials available [here](https://github.com/cfenollosa/os-tutorial). The system already has thread support and will soon be much more complete!

<hr> 

## Build dependencies

You will need:

![gnu_bash](https://img.shields.io/badge/GNU%20Bash-4EAA25?style=for-the-badge&logo=GNU%20Bash&logoColor=white)
![github](https://img.shields.io/badge/GitHub-8117EB?style=for-the-badge&logo=github&logoColor=white)
![qemu](https://img.shields.io/badge/Qemu-0A0A0A?style=for-the-badge&logo=qemu&logoColor=white)

To build Lunix, you must build the cross-compiler tools. For that, you first need to install the dependencies to build gcc, binutils and the system.

### macOS

On macOS, you must install the dependencies using the brew tool:

```sh
brew install gmp mpfr libmpc gcc
```

### Linux

On `Ubuntu`, `Debian` or `Pop!_OS`, use:

```
sudo apt install gcc libmpc-dev nasm fasm qemu-system qemu-system-i386
```

### Windows (WSL2)

On Windows, you must first install WSL2 to build Lunix. After that, you must install Ubuntu through the Windows Store. Then use the command below to install the dependencies:

```
sudo apt install gcc libmpc-dev nasm fasm qemu-system qemu-system-i386
```

<hr>

## Building the cross-compiler and tools

You will need to build binutils and a cross-compiled gcc, and put them into `/usr/local/lunixgcc`. To do that, use:

```
export PREFIX="/usr/local/lunixgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"
```

### Building binutils

To build ```binutils```, use the follow shell commands:

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

Use the commands below to build ```GCC```:

```sh
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-12.1.0/gcc-12.1.0.tar.xz
xz -d gcc-12.1.0.tar.xz
tar xf gcc-12.1.0.tar 
mkdir gcc-build
cd gcc-build
../gcc-12.1.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
make all-gcc 
make all-target-libgcc 
make install-gcc 
make install-target-libgcc 
```

That's it! You should have all the GNU binutils and the compiler at `/usr/local/lunixgcc/bin`, prefixed by `i386-elf-` to avoid collisions with your system's compiler and binutils.

You may want to add the `$PATH` to your `.bashrc`. To do that, use the command bellow:

```sh
export LUNIX="/usr/local/lunixgcc"
echo 'export PATH="$LUNIX/bin:$PATH"' >> /home/your_user/.bashrc
```

<hr>

## Build the system

To build Lunix, run make on the kernel source folder. To do that, use:

```
cd kern
make
```

To test the system, use:

```
make run
```

