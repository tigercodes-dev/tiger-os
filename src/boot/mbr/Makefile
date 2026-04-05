.PHONY: all mbr clean
.SILENT:

all: mbr

mbr: $(BUILD)/mbr.bin

$(BUILD)/mbr.bin: mbr.asm
	mkdir -p $(@D)
	nasm -f bin -o $@ $<
	echo "ASM:  $< --> $@"
	echo "Finished building $@"

clean:
	rm -f $(BUILD)/mbr.bin