.PHONY: all bootsector clean
.SILENT:

all: kernel

kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin: main.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM: $< --> $@"
	echo "Finished Building $@"

clean:
	rm -f $(BUILD)/kernel.bin
