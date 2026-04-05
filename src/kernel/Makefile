TARGET_ASMFLAGS += -f elf
TARGET_CCFLAGS += -ffreestanding -nostdlib
TARGET_LDLIBS += -lgcc
TARGET_LDFLAGS += -T linker.ld -nostdlib

BUILD_KERNEL := $(BUILD)/kernel

SRC_C := $(shell find -name "*.c")
SRC_ASM := $(shell find -name "*.asm")
OBJ_C := $(patsubst %.c, $(BUILD_KERNEL)/c/%.obj, $(SRC_C))
OBJ_ASM := $(patsubst %.asm, $(BUILD_KERNEL)/asm/%.obj, $(SRC_ASM))
HEADERS_ASM := $(shell find -name "*.inc")
HEADERS_C := $(shell find -name "*.h")

.PHONY: all kernel clean always
.SILENT:

all: kernel

kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin: $(OBJ_ASM) $(OBJ_C)
	mkdir -p $(@D)
	$(TARGET_LD) $(TARGET_LDFLAGS) -Wl,-Map=$(BUILD_KERNEL)/kernel.map -o $@ $^ $(TARGET_LDLIBS)
	echo "LD: Linked object files --> $@"
	echo "Finished Building $@"

$(BUILD_KERNEL)/asm/%.obj: %.asm $(HEADERS_ASM)
	mkdir -p $(@D)
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<
	echo "ASM: $< --> $@"

$(BUILD_KERNEL)/c/%.obj: %.c $(HEADERS_C)
	mkdir -p $(@D)
	$(TARGET_CC) $(TARGET_CCFLAGS) -c -o $@ $<
	echo "CC: $< --> $@"

i686/isr_gen/isr_gen.c i686/isr_gen/isr_gen.inc: $(SCRIPTS)/isr_gen.py
	cd $(ROOT) && python3 $(SCRIPTS)/isr_gen.py
	echo "Generated ISRs"

clean:
	rm -f $(BUILD)/kernel.bin
