# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- FAT32 support
- Keyboard and mouse drivers
- Disk creation tools for partitioning & adding files more easily

## [0.0.2] - 2026-04-06

### Changed

- Improved MBR interface for booting
    - Automatically select partition if there is only one.
    - Reset registers before loading boot sector.
- Split partition table into multiple files.

### Added

- Added this changelog to log future updates.
- Added copyright messages.

## [0.0.1] - 2026-04-05

### Added

- Implemented QEMU logging.
- Implemented virtual file system (VFS) abstraction and improved QEMU debug logging.
- Master Boot Record (MBR) with partition selection system.

### Changed

- Increased disk size from 32MiB to 64MiB
- Changed sectors per cluster from 4 to 8

### Fixed

- Fixed non-critical boot sector bug where small portion of code dealing with bugs in some BIOSes (not QEMU) is skipped.

## [0.0.1-beta.2] - 2026-04-04

### Added

- Implemented Global Descriptor Table in the kernel.
- Implemented Interrupt Descriptor Table and interrupt handling in the kernel.
- Implemented PIC hardware interrupt handling.
- Added more information to `README.md`

### Changed

- Moved kernel IO port functions from `hardware.h` to `i686/ports.h`.
- Changed IO port names to match the C standard libraries:
    - `outbyte` & `inbyte` → `outb` & `inb`
    - `outword` & `inword` → `outw` & `inw`
    - `outdword` & `indword` → `outl` & `inl`

## [0.0.1-beta] - 2026-04-03

_First beta release._

### Added

- Boot sector loading the kernel loader (KRNLD).
- Kernel loader FAT16 driver.
- Kernel loader loading kernel file.
- i686 GCC cross compiler.
