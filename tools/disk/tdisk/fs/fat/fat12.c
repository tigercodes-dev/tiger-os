#include "fat12.h"
#include "common.h"
#include "../../rand.h"
#include "../../diskutil.h"
#include "../../mathutil.h"
#include <memory.h>
#include <ctype.h>

static const FAT12_16VBR VBR_EMPTY = {0};

#define bootrec vbr.fat_boot_record

int FAT12_format(FILE* file, uint32_t partition_start, uint32_t partition_size, bool quick, const char* vol_label) {
    if (cdiv(partition_size, 128) > 4084) {
        fprintf(stderr, "\e[31;1mError:\e[0m The disk is larger than FAT12 maximum size.\n");
        return 2;
    }

    fseek(file, partition_start * SECTOR_SIZE, SEEK_SET);

    FAT12_16VBR vbr;
    fread(&vbr, 1, sizeof(FAT12_16VBR), file);
    fseek(file, -(sizeof(FAT12_16VBR)), SEEK_CUR);

    char fat_vol_lbl[11];
    memset(fat_vol_lbl, ' ', 11);
    for (int i = 0; vol_label[i] && i < 11; i++) {
        fat_vol_lbl[i] = toupper((unsigned char)vol_label[i]);
    }

    // Initialize VBR with default values
    memcpy(bootrec.jmp_instruction, (uint8_t[]){0xEB, 0x3C, 0x90}, 3);
    memcpy(bootrec.oem_identifier, "TDISKFAT", 8);
    bootrec.bytes_per_sector = SECTOR_SIZE;
    bootrec.sectors_per_cluster = alignpow2(cdiv(partition_size, 4084));
    bootrec.reserved_sectors = bootrec.sectors_per_cluster;
    bootrec.fat_count = 2;
    bootrec.root_dir_entries = 224;
    bootrec.total_sectors = partition_size > 65535 ? 0 : (uint16_t)partition_size;
    bootrec.media_descriptor = 0xF8;
    bootrec.sectors_per_fat = align(partition_size / bootrec.sectors_per_cluster / 342, bootrec.sectors_per_cluster);
    bootrec.sectors_per_track = SECTORS_PER_TRACK;
    bootrec.heads = HEADS;
    bootrec.hidden_sectors = partition_start;
    bootrec.large_sector_count = partition_size <= 65535 ? 0 : partition_size;

    bootrec.drive_num = 0x00;
    bootrec.__reserved = 0;
    bootrec.signature = 0x29;
    bootrec.serial_num = rand_32();
    memcpy(bootrec.volume_label, vol_label, 11);
    memcpy(bootrec.system_id, "FAT12   ", 8);
    vbr.vbr_bytes.boot_signature = 0xAA55;
    memcpy(vbr.vbr_bytes.boot_code + sizeof(FAT12_16BootRecord), (uint8_t[]){0xEB, 0xFE}, 2);

    if (fwrite(&vbr, 1, sizeof(FAT12_16VBR), file) != sizeof(FAT12_16VBR)) {
        fprintf(stderr, "\e[33;1mWarning:\e[0m Not all of the boot data was able to be written to the disk.\n");
    }

    printf("Wrote boot data.\n");

    uint8_t sector[SECTOR_SIZE] = {0};
    ((uint32_t*)sector)[0] = 0x00FF0FFF;

    for (int i = 0; i < bootrec.fat_count; i++) {
        fwrite(sector, 1, SECTOR_SIZE, file);
        for (int j = 0; j < bootrec.sectors_per_fat - 1; j++) {
            fwrite(ZERO_BLOCK, 1, SECTOR_SIZE, file);
        }
    }

    if (!quick) {
        for (int i = 0; i < partition_size - 1; i++) {
            fwrite(ZERO_BLOCK, 1, SECTOR_SIZE, file);
        }
    }

    fseek(file, 0, SEEK_SET);

    printf("Formatted partition to FAT12.\n");

    return 0;
}

