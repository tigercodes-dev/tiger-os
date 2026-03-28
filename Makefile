export BUILD := $(abspath build)

# 32M is recommended minimum size for disk
DISK_SIZE := 32M
DISK_FILE ?= $(BUILD)/TigerOS.img

.PHONY: all disk bootsector kernel clean
.SILENT:

all: disk

# Disk Image

disk: $(DISK_FILE)

$(DISK_FILE): bootloader kernel
	mkdir -p $(@D)
	head -c $(DISK_SIZE) /dev/zero > $@
	mkfs.fat -F 16 -n "TIGER OS" $@ > /dev/zero
	echo "Initialized FAT16"
	dd if=$(BUILD)/bootsector.bin of=$@ conv=notrunc > /dev/zero 2>&1
	mcopy -i $@ $(BUILD)/krnld.bin "::/KRNLD.SYS"
	mmd -i $@ "::/SYSTEM"
	mcopy -i $@ $(BUILD)/kernel.bin "::/SYSTEM/KERNEL.SYS"
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

clean:
	$(MAKE) -C src/boot clean
	$(MAKE) -C src/kernel clean
	rm -rf $(BUILD)/*
