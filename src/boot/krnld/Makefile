.PHONY: all bootsector clean
.SILENT:

TARGET_ASMFLAGS += -f elf
TARGET_CCFLAGS += -ffreestanding -nostdlib -c
TARGET_LDLIBS += -lgcc
TARGET_LDFLAGS += -T linker.ld -nostdlib

KRNLD_BUILD := $(BUILD)/krnld

SOURCES_ASM := $(wildcard *.asm)
SOURCES_C := $(wildcard *.c)
HEADERS_C := $(wildcard *.h)

OBJ_ASM := $(patsubst %.asm, $(KRNLD_BUILD)/asm/%.o, $(SOURCES_ASM))
OBJ_C := $(patsubst %.c, $(KRNLD_BUILD)/c/%.o, $(SOURCES_C))

all: krnld

krnld: $(BUILD)/krnld.bin

$(BUILD)/krnld.bin: $(OBJ_ASM) $(OBJ_C)
	mkdir -p $(@D)
	$(TARGET_LD) $(TARGET_LDFLAGS) -Wl,-Map=$(KRNLD_BUILD)/krnld.map -o $@ $^ $(TARGET_LDLIBS)
	echo "LD: Linked object files --> $@"
	echo "Finished Building $@"

$(KRNLD_BUILD)/asm/%.o: %.asm
	mkdir -p $(@D)
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<
	echo "ASM: $< --> $@"

$(KRNLD_BUILD)/c/%.o: %.c $(HEADERS_C)
	mkdir -p $(@D)
	$(TARGET_CC) $(TARGET_CCFLAGS) -o $@ $<
	echo "CC: $< --> $@"

clean:
	rm -f $(BUILD)/kernel.bin
