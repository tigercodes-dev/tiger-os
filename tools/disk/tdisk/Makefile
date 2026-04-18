.PHONY: all tdisk install install-bin install-man clean
.SILENT:

EXEC := $(BUILD)/tools/tdisk

SRC_C := $(shell find -name "*.c")
HEADERS_C := $(shell find -name "*.h")

PREFIX ?= /usr/local
DESTDIR ?= /
INSTALL_PATH = $(abspath $(DESTDIR)/$(PREFIX)/sbin)

MAN_1 := $(wildcard $(ROOT)/docs/manpages/*.1)
MAN_8 := $(wildcard $(ROOT)/docs/manpages/*.8)
MAN_OUT_1 := $(patsubst $(ROOT)/docs/manpages/%.1, $(DESTDIR)/$(PREFIX)/share/man/man1/%.1.gz, $(MAN_1))
MAN_OUT_8 := $(patsubst $(ROOT)/docs/manpages/%.8, $(DESTDIR)/$(PREFIX)/share/man/man8/%.8.gz, $(MAN_8))

all: tdisk

tdisk: $(EXEC)

$(EXEC): $(SRC_C) $(HEADERS_C)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(SRC_C) -o $@
	echo "$@ - TDISK tool is ready."

install: install-bin install-man

install-bin:
	cp $(EXEC) $(INSTALL_PATH)
	echo "Installed TDISK to $(INSTALL_PATH)"

install-man: $(MAN_OUT_1) $(MAN_OUT_8)
	mandb > /dev/null
	echo "Installed and updated man pages."

$(DESTDIR)/$(PREFIX)/share/man/man1/%.1.gz: $(ROOT)/docs/manpages/%.1
	mkdir -p $(@D)
	gzip -c $< > $@
	echo "GZIP $< --> $@"

$(DESTDIR)/$(PREFIX)/share/man/man8/%.8.gz: $(ROOT)/docs/manpages/%.8
	mkdir -p $(@D)
	gzip -c $< > $@
	echo "GZIP $< --> $@"

clean:
	rm -f $(EXEC)
