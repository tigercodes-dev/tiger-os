.PHONY: all bootsector clean
.SILENT:

all: bootsector

bootsector: $(BUILD)/bootsector.bin

$(BUILD)/bootsector.bin: bootsector.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM:  $< --> $@"
	echo "Finished building $@"

clean:
	rm -f $(BUILD)/bootsector.bin
