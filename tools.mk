JOBS ?= 4

BINUTILS_VER := 2.46.0
BINUTILS_URL := https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VER).tar.xz

GCC_VER := 15.2.0
GCC_URL := https://ftp.gnu.org/gnu/gcc/gcc-15.2.0/gcc-$(GCC_VER).tar.xz

TOOLCHAIN := $(abspath toolchain)
TOOLCHAIN_PREFIX := $(TOOLCHAIN)/$(TARGET)
export PATH := $(TOOLCHAIN_PREFIX)/bin:$(PATH)

.PHONY: toolchain tools-binutils tools-gcc clean-toolchain

toolchain: tools-binutils tools-gcc

BINUTILS_SRC := $(TOOLCHAIN)/binutils-$(BINUTILS_VER)
BINUTILS_BUILD := $(TOOLCHAIN)/binutils-$(BINUTILS_VER)-build

tools-binutils: $(TOOLCHAIN_PREFIX)/bin/i686-elf-ld

$(TOOLCHAIN_PREFIX)/bin/i686-elf-ld: $(BINUTILS_SRC).tar.xz
	cd $(TOOLCHAIN) && tar -xf binutils-$(BINUTILS_VER).tar.xz
	mkdir -p $(BINUTILS_BUILD)
	cd $(BINUTILS_BUILD) && CCFLAGS= ASMFLAGS= CC= LD= ASM= LDFLAGS= LDLIBS= ../binutils-$(BINUTILS_VER)/configure \
		--prefix="$(TOOLCHAIN_PREFIX)" --target=$(TARGET) --with-sysroot --disable-nls --disable-werror
	$(MAKE) -j $(JOBS) -C $(BINUTILS_BUILD)
	$(MAKE) -C $(BINUTILS_BUILD) install

$(BINUTILS_SRC).tar.xz:
	mkdir -p $(TOOLCHAIN)
	cd $(TOOLCHAIN) && wget $(BINUTILS_URL)

GCC_SRC := $(TOOLCHAIN)/gcc-$(BINUTILS_VER)
GCC_BUILD := $(TOOLCHAIN)/gcc-$(BINUTILS_VER)-build

tools-gcc: $(TOOLCHAIN_PREFIX)/bin/i686-elf-gcc

$(TOOLCHAIN_PREFIX)/bin/i686-elf-gcc: $(TOOLCHAIN_PREFIX)/bin/i686-elf-ld $(GCC_SRC).tar.xz
	cd $(TOOLCHAIN) && tar -xf gcc-$(GCC_VER).tar.xz
	mkdir -p $(GCC_BUILD)
	cd $(GCC_BUILD) && CCFLAGS= ASMFLAGS= CC= LD= ASM= LDFLAGS= LDLIBS= ../gcc-$(GCC_VER)/configure \
		--prefix="$(TOOLCHAIN_PREFIX)" --target=$(TARGET) --disable-nls --enable-languages=c --without-headers
	$(MAKE) -j $(JOBS) -C $(GCC_BUILD) all-gcc all-target-libgcc
	$(MAKE) -C $(GCC_BUILD) install-gcc install-target-libgcc

$(GCC_SRC).tar.xz:
	mkdir -p $(TOOLCHAIN)
	cd $(TOOLCHAIN) && wget $(GCC_URL)

clean-toolchain:
	rm -rf $(TOOLCHAIN)/*
