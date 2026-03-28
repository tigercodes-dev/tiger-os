.PHONY: all bootsector clean
.SILENT:

all: krnld

krnld: $(BUILD)/krnld.bin

$(BUILD)/krnld.bin: entry.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM: $< --> $@"
	echo "Finished Building $@"

clean:
	rm -f $(BUILD)/kernel.bin
