export BUILD := $(abspath build)

# 32M is recommended minimum size for disk
DISK_SIZE ?= 32M
DISK_FILE ?= $(BUILD)/TigerOS.img

.PHONY: all disk bootsector build-dir clean
.SILENT:

all: disk

# Disk Image

disk: $(DISK_FILE)

$(DISK_FILE): bootsector build-dir
	head -c $(DISK_SIZE) /dev/zero > $@
	dd if=$(BUILD)/bootsector.bin of=$@ conv=notrunc > /dev/zero 2>&1
	echo "$@ is ready."

# Bootloader

bootsector: $(BUILD)/bootsector.bin

$(BUILD)/bootsector.bin: src/boot/bootsector.asm build-dir
	nasm -f bin -o $@ $<
	echo "  ASM:  $< --> $@"
	echo "\nFinished building $@"

# Special Targets

build-dir:
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)/*
