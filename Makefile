ifdef BUILD
export BUILD
else
export BUILD := $(abspath build)
endif

export ROOT := $(abspath .)
export SCRIPTS := $(abspath scripts)

export TIGER_OS_VER = 0.2.0

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

export PATH := $(BUILD)/tools:$(PATH)

DISK_SIZE := 64M
DISK_FILE ?= $(BUILD)/TigerOS.img

.PHONY: all disk tools tdisk tdisk-install mbr ospartition bootloader bootsector krnld kernel clean
.SILENT:

all: tools disk

include tools.mk

# Disk Image

disk: $(DISK_FILE)

$(DISK_FILE): mbr ospartition
	mkdir -p $(@D)
	head -c $(DISK_SIZE) /dev/zero > $@
	tdisk $@ mbr $(BUILD)/mbr.bin -q -x > /dev/zero
	tdisk $@ create-partition primary -t "FAT32 CHS" > /dev/zero
	tdisk $@ write-sectors 1 1 -i build/mbr-ext.bin > /dev/zero
	dd if=$(BUILD)/ospart.img of=$@ bs=512 seek=63 conv=notrunc > /dev/zero 2>&1
	echo "Added TigerOS partition to disk."
	echo "$@ is ready."

# Master Boot Record

mbr:
	$(MAKE) -C src/boot/mbr

# TigerOS partition

ospartition: $(BUILD)/ospart.img

$(BUILD)/ospart.img: bootloader kernel
	head -c $(DISK_SIZE) /dev/zero > $@
	truncate -s -32256 $@
	mkfs.fat -F 32 -n "TIGER OS" -g 16/63 $@ > /dev/zero
	echo "Initialized FAT32"
	dd if=$(BUILD)/bootsector.bin of=$@ conv=notrunc > /dev/zero 2>&1
	dd if=$(BUILD)/bootsector.bin bs=512 seek=6 of=$@ conv=notrunc > /dev/zero 2>&1
	mcopy -i $@ $(BUILD)/krnld.bin "::/KRNLD.SYS"
	mattrib -i $@ +h +s "::/KRNLD.SYS"
	mmd -i $@ "::/SYSTEM"
	mcopy -i $@ $(BUILD)/kernel.bin "::/SYSTEM/KERNEL.SYS"
	mattrib -i $@ +h +s "::/SYSTEM/KERNEL.SYS"
	mmd -i $@ "::/HOME"
	mmd -i $@ "::/HOME/DOCS"
	mcopy -i $@ disk-files/lorem.txt "::/HOME/DOCS/LOREM.TXT"
	echo "Finished adding files to disk."
	echo "$@ is ready."

# Bootloader

bootloader: bootsector krnld

bootsector:
	$(MAKE) -C src/boot/bootsector

krnld:
	$(MAKE) -C src/boot/krnld

# Kernel

kernel:
	$(MAKE) -C src/kernel

# Tools

tools: tdisk

tdisk:
	$(MAKE) -C tools/disk/tdisk

tdisk-install:
	export PREFIX
	export DESTDIR
	$(MAKE) -C tools/disk/tdisk install

# Special Targets

help:
	echo "TigerOS V$(TIGER_OS_VER) <https://github.com/tigercodes-dev/tiger-os>"
	echo
	echo "TigerOS  Copyright (C) 2026  tigercodes-dev <https://github.com/tigercodes-dev>"
	echo "This program comes with ABSOLUTELY NO WARRANTY."
	echo "This is free software, and you are welcome to redistribute it under certain conditions."
	echo "See COPYING for more information."
	echo
	echo "Makefile Information"
	echo
	echo "Build Dir: $(BUILD)"
	echo "Target Architecture: $(TARGET)"
	echo
	echo "Makefile Targets:"
	echo "\thelp - shows this help message"
	echo "\tversion - shows a version message"
	echo
	echo "\tall - builds everything"
	echo
	echo "\tdisk - builds the disk image"
	echo "\tmbr - build the Master Boot Record"
	echo "\ttospartition - build the TigerOS disk partition"
	echo "\tbootloader - builds the bootloader (boot sector and kernel loader)"
	echo "\tbootsector - builds the boot sector"
	echo "\tkrnld - builds the kernel loader"
	echo "\tkernel - builds the kernel"
	echo "\tclean - clears the build directory"
	echo
	echo "\ttdisk - builds the TDISK tool"
	echo "\ttdisk-install - installs the TDISK tool (may require sudo when installing to system directory)"
	echo
	echo "\ttoolchain - builds the toolchain"
	echo "\ttools-binutils - builds binutils, version: $(BINUTILS_VER)"
	echo "\ttools-gcc - builds the gcc cross compiler, version: $(GCC_VER)"
	echo "\tclean-toolchain - clears the toolchain directory"
	echo

version:
	echo "TigerOS V$(TIGER_OS_VER) <https://github.com/tigercodes-dev/tiger-os>"
	echo
	echo "TigerOS  Copyright (C) 2026  tigercodes-dev <https://github.com/tigercodes-dev>"
	echo "This program comes with ABSOLUTELY NO WARRANTY."
	echo "This is free software, and you are welcome to redistribute it under certain conditions."
	echo "See COPYING for more information."
	echo

clean:
	$(MAKE) -C src/boot/mbr clean
	$(MAKE) -C src/boot/bootsector clean
	$(MAKE) -C src/boot/krnld clean
	$(MAKE) -C src/kernel clean
	$(MAKE) -C tools/disk/tdisk clean
	rm -rf $(BUILD)/*
	echo "Cleaned build directory."
