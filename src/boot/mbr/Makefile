.PHONY: all mbr clean
.SILENT:

all: mbr extended-code

mbr: $(BUILD)/mbr.bin

$(BUILD)/mbr.bin: mbr.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM:  $< --> $@"
	echo "Finished building $@"

extended-code: $(BUILD)/mbr-ext.bin

$(BUILD)/mbr-ext.bin: mbr-ext.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM:  $< --> $@"
	echo "Finished building $@"

clean:
	rm -f $(BUILD)/mbr.bin