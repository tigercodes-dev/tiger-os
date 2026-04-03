# TigerOS

TigerOS is a simple operating system made from scratch. 

Currently, it is still in early beta.

## Requirements

Before building the OS, you need a few tools.

- `nasm` - assembler
- `make` - build tool
- `qemu` - virtualization software for testing the OS
- `dosfstools` - creating the filesystem on the disk
- `mtools` - managing files on the disk

You can install them with these commands on Ubuntu Linux:

```
sudo apt update
sudo apt install nasm make qemu dosfstools mtools
```

## Building

### Toolchain

Before building, you need to build the GCC cross compiler and binutils.
Run `make toolchain` to build the toolchain. This might take a few minutes.

### Code

Simply run `make` to compile the OS code automatically using the Makefile.

You can also specify specific targets to build like `disk` or `kernel`.
To see a list of targets, use `make help`.

If you want to clean the build directory, run `make clean`

## Testing

Use the `./run` script to run TigerOS in QEMU.

## Debugging

Use the `./debug` script to run TigerOS in QEMU with debugging.

Then, in another terminal, connect to QEMU in gdb using `target remote :1234`

- Use `break *[address]` to set a breakpoint.
- Use `c` to continue execution unt.mine the registers.
- Use `x [address]` to examine memory. You can specify options with a slash. (ex. `x/2xb` prints two bytes)
