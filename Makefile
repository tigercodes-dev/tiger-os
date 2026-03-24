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
	mkfs.fat -F 16 -n "TIGER OS" $@ > /dev/zero
	echo "Initialized FAT16"
	dd if=$(BUILD)/bootsector.bin of=$@ conv=notrunc > /dev/zero 2>&1
	mcopy -i $@ test.txt "::/TEST.TXT"
	echo "Finished adding files to disk."
	echo "$@ is ready."

# Bootloader

bootsector: $(BUILD)/bootsector.bin

$(BUILD)/bootsector.bin: build-dir
	$(MAKE) -C src/boot

# Special Targets

build-dir:
	mkdir -p $(BUILD)

clean:
	$(MAKE) -C src/boot clean
	rm -rf $(BUILD)/*
