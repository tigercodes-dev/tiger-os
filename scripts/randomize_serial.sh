#!/bin/bash

OFFSET_DISK_ID=$((0x1B8))
BYTES_DISK_ID=4

OFFSET_VOLUME_ID=$((0x7E27))
BYTES_VOLUME_ID=4

dd if=/dev/urandom of=build/TigerOS.img bs=1 count=${BYTES_DISK_ID} conv=notrunc seek=${OFFSET_DISK_ID} &>/dev/null
dd if=/dev/urandom of=build/TigerOS.img bs=1 count=${BYTES_VOLUME_ID} conv=notrunc seek=${OFFSET_VOLUME_ID} &>/dev/null
