.PHONY: all tdisk install install-bin install-man install-doc clean
.SILENT:

EXEC := $(BUILD)/tools/tdisk

SRC_C := $(shell find -name "*.c")
HEADERS_C := $(shell find -name "*.h")

PREFIX ?= /usr/local

MAN_1 := $(wildcard $(ROOT)/docs/manpages/*.1)
MAN_8 := $(wildcard $(ROOT)/docs/manpages/*.8)
MAN_OUT_1 := $(patsubst $(ROOT)/docs/manpages/%.1, $(PREFIX)/share/man/man1/%.1.gz, $(MAN_1))
MAN_OUT_8 := $(patsubst $(ROOT)/docs/manpages/%.8, $(PREFIX)/share/man/man8/%.8.gz, $(MAN_8))

DOCS := $(wildcard $(ROOT)/docs/tdisk-help/*)
DOCS_OUT := $(patsubst $(ROOT)/docs/tdisk-help/%, $(PREFIX)/share/doc/tdisk/%, $(DOCS))

all: tdisk

tdisk: $(EXEC)

$(EXEC): $(SRC_C) $(HEADERS_C)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(SRC_C) -o $@
	echo "$@ - TDISK tool is ready."

install: install-bin install-man install-doc
	echo "Installed all TDISK components to $(PREFIX)."

install-bin:
	install -m 755 $(EXEC) $(PREFIX)/sbin
	echo "Installed TDISK to $(PREFIX)/sbin"

install-man: $(MAN_OUT_1) $(MAN_OUT_8)
	mandb > /dev/null
	echo "Installed and updated man pages."

$(PREFIX)/share/man/man1/%.1.gz: $(ROOT)/docs/manpages/%.1
	install -m 755 -d $(@D)
	echo "INSTALL $(@D)"
	gzip -c $< > $@
	chmod 644 $@
	echo "GZIP $< --> $@"

$(PREFIX)/share/man/man8/%.8.gz: $(ROOT)/docs/manpages/%.8
	install -m 755 -d $(@D)
	echo "INSTALL $(@D)"
	gzip -c $< > $@
	chmod 644 $@
	echo "GZIP $< --> $@"

install-doc: $(DOCS_OUT)
	echo "Installed TDISK docs to $(PREFIX)/share/doc/tdisk."

$(PREFIX)/share/doc/tdisk/%: $(ROOT)/docs/tdisk-help/%
	install -m 755 -d $(@D)
	echo "INSTALL $(@D)"
	install -m 644 $< $@
	echo "INSTALL $< --> $@"

clean:
	rm -f $(EXEC)
