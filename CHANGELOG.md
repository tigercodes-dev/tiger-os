# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased Updates

You can view the main unreleased updates [here](https://github.com/tigercodes-dev/tiger-os/compare/main..develop).

You can also view unreleased TDISK updates [here](https://github.com/tigercodes-dev/tiger-os/compare/develop...tdisk).

_Be warned that these unreleased updates may contain bugs! Stable releases are on the main branch._

## [0.2.2] - 2026-04-26

### Added
- New selectable TDISK partition types
- New commands for listing partitions, marking partitions as active, and deleting partitions
- Update release date when using `make help` or `make version`

### Changed
- Moved TDISK help message to separate documentation file: `/usr/local/share/doc/tdisk/tdisk-commands.txt`. The help message now also opens in less to allow easier viewing.

### Fixed
- Fixed bugs causing segmentation faults when parsing TDISK arguments.

## [0.2.1] - 2026-04-18

### Added

- Added TDISK read/write sectors tools.
- Updated TDISK --help info.
- Added Man Pages for TDISK.
- TDISK install command to install tdisk to the machine.

## [0.2.0] - 2026-04-13

### Added

- Keyboard scancode to character translation
- Typing on screen.
- More descriptive MBR partition selection interface.

### Changed

- Renamed `ports.h` to `io.h` in kernel.
- Moved disk files to `disk-files` directory.
- Changed `test.txt` to `lorem.txt` with lorem ipsum text.

## [0.1.0] - 2026-04-11

### Added

- FAT32 Support
- Started new TDISK tool. Development will occur in the `tdisk` branch. Implemented MBR partitioning tools for TDISK.
- Added install dependencies command for homebrew in `README.md`

### Changed

- Bootloader now uses hardcoded cluster value instead of searching on the disk.
- The Arch Linux (pacman) install dependencies command now includes `dosfstools` which is not installed by default on Arch.
- Moved `README.md` images to `docs` folder.

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

[0.2.2]: https://github.com/tigercodes-dev/tiger-os/compare/v0.2.1...v0.2.2
[0.2.1]: https://github.com/tigercodes-dev/tiger-os/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/tigercodes-dev/tiger-os/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/tigercodes-dev/tiger-os/compare/v0.0.2...v0.1.0
[0.0.2]: https://github.com/tigercodes-dev/tiger-os/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/tigercodes-dev/tiger-os/compare/v0.0.1-beta.2...v0.0.1
[0.0.1-beta.2]: https://github.com/tigercodes-dev/tiger-os/compare/v0.0.1-beta...v0.0.1-beta.2
[0.0.1-beta]: https://github.com/tigercodes-dev/tiger-os/releases/tag/v0.0.1-beta