.PHONY: all bootsector build-dir clean
.SILENT:

all: bootsector

bootsector: $(BUILD)/bootsector.bin

$(BUILD)/bootsector.bin: bootsector.asm
	nasm -f bin -o $@ $<
	echo "ASM:  $< --> $@"
	echo "Finished building $@"

build-dir:
	mkdir -p $(BUILD)

clean:
	rm -f $(BUILD)/bootsector.bin
