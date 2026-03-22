# TigerOS

TigerOS is a simple operating system made from scratch. 

Currently, it is still in early beta.

## Requirements

Before building the OS, you need a few tools.

- `nasm` - assembler
- `make` - build tool
- `qemu` - virtualization software for testing the OS

You can install them with these commands on Ubuntu Linux:

```
$ sudo apt update
$ sudo apt install nasm make qemu
```

## Building

Simply run `make` to build the OS automatically using the Makefile.

If you want to clean the build directory, run `make clean`

## Testing

Use the `run` script to run TigerOS in QEMU.
