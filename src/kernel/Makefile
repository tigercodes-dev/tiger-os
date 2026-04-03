TARGET_ASMFLAGS += -f elf
TARGET_CCFLAGS += -ffreestanding -nostdlib
TARGET_LDLIBS += -lgcc
TARGET_LDFLAGS += -T linker.ld -nostdlib

BUILD_KERNEL := $(BUILD)/kernel

SRC_C := $(wildcard *.c)
SRC_ASM := $(wildcard *.asm)
OBJ_C := $(patsubst %.c, $(BUILD_KERNEL)/c/%.obj, $(SRC_C))
OBJ_ASM := $(patsubst %.asm, $(BUILD_KERNEL)/asm/%.obj, $(SRC_ASM))

.PHONY: all kernel clean always
.SILENT:

all: kernel

kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin: $(OBJ_ASM) $(OBJ_C)
	mkdir -p $(@D)
	$(TARGET_LD) $(TARGET_LDFLAGS) -Wl,-Map=$(BUILD_KERNEL)/kernel.map -o $@ $^ $(TARGET_LDLIBS)
	echo "LD: Linked object files --> $@"
	echo "Finished Building $@"

$(BUILD_KERNEL)/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<
	echo "ASM: $< --> $@"

$(BUILD_KERNEL)/c/%.obj: %.c
	mkdir -p $(@D)
	$(TARGET_CC) $(TARGET_CCFLAGS) -c -o $@ $<
	echo "CC: $< --> $@"

clean:
	rm -f $(BUILD)/kernel.bin
