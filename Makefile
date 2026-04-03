ifdef BUILD
export BUILD
else
export BUILD := $(abspath build)
endif

export TIGER_OS_VER = 0.0.1-beta

export ASM := nasm
export CC := gcc
export LD := gcc

export ASMFLAGS :=
export CCFLAGS := -std=c99 -g
export LDFLAGS :=
export LDLIBS :=

export TARGET := i686-elf
export TARGET_ASM := nasm
export TARGET_CC := $(TARGET)-gcc
export TARGET_LD := $(TARGET)-gcc

export TARGET_ASMFLAGS :=
export TARGET_CCFLAGS := -std=c99 -g
export TARGET_LDFLAGS :=
export TARGET_LDLIBS :=

# 32M is recommended minimum size for disk
DISK_SIZE := 32M
DISK_FILE ?= $(BUILD)/TigerOS.img

.PHONY: all disk bootloader bootsector krnld kernel clean
.SILENT:

all: disk

include tools.mk

# Disk Image

disk: $(DISK_FILE)

$(DISK_FILE): bootloader kernel
	mkdir -p $(@D)
	head -c $(DISK_SIZE) /dev/zero > $@
	mkfs.fat -F 16 -n "TIGER OS" $@ > /dev/zero
	echo "Initialized FAT16"
	dd if=$(BUILD)/bootsector.bin of=$@ conv=notrunc > /dev/zero 2>&1
	mcopy -i $@ $(BUILD)/krnld.bin "::/KRNLD.SYS"
	mattrib -i $@ +h +s "::/KRNLD.SYS"
	mmd -i $@ "::/SYSTEM"
	mcopy -i $@ $(BUILD)/kernel.bin "::/SYSTEM/KERNEL.SYS"
	mattrib -i $@ +h +s "::/SYSTEM/KERNEL.SYS"
	mcopy -i $@ test.txt "::/SYSTEM/TEST.TXT"
	echo "Finished adding files to disk."
	echo "$@ is ready."

# Bootloader

bootloader: bootsector krnld

bootsector:
	$(MAKE) -C src/boot

krnld:
	$(MAKE) -C src/boot/krnld

# Kernel

kernel:
	$(MAKE) -C src/kernel

# Special Targets

help:
	echo "TigerOS V$(TIGER_OS_VER) <https://github.com/tigercodes-dev/tiger-os>"
	echo "By tigercodes-dev <https://github.com/tigercodes-dev>"
	echo
	echo "Build Dir: $(BUILD)"
	echo "Target Architecture: $(TARGET)"
	echo
	echo "Makefile Targets:"
	echo "\thelp - shows this help message"
	echo "\tversion - shows a version message"
	echo
	echo "\tall - builds everything"
	echo "\tdisk - builds the disk image"
	echo "\tbootloader - builds the bootloader (boot sector and kernel loader)"
	echo "\tbootsector - builds the boot sector"
	echo "\tkrnld - builds the kernel loader"
	echo "\tkernel - builds the kernel"
	echo "\tclean - clears the build directory"
	echo
	echo "\ttoolchain - builds the toolchain"
	echo "\ttools-binutils - builds binutils, version: $(BINUTILS_VER)"
	echo "\ttools-gcc - builds the gcc cross compiler, version: $(GCC_VER)"
	echo "\tclean-toolchain - clears the toolchain directory"
	echo

version:
	echo "TigerOS by tigercodes-dev <https://github.com/tigercodes-dev>"
	echo "Version $(TIGER_OS_VER)"

clean:
	$(MAKE) -C src/boot clean
	$(MAKE) -C src/boot/krnld clean
	$(MAKE) -C src/kernel clean
	rm -rf $(BUILD)/*
	echo "Cleaned build directory."
