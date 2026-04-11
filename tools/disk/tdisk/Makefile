.PHONY: all tdisk clean
.SILENT:

EXEC := $(BUILD)/tools/tdisk

SRC_C := $(shell find -name "*.c")
HEADERS_C := $(shell find -name "*.h")

all: tdisk

tdisk: $(EXEC)

$(EXEC): $(SRC_C) $(HEADERS_C)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(SRC_C) -o $@
	echo "$@ - TDISK tool is ready."

clean:
	rm -f $(EXEC)
