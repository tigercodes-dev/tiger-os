#include "disk.h"
#include "hardware.h"


bool load_disk(DISK* disk, uint8_t drive) {
    uint8_t drive_type;
    uint16_t cyls, secs, heads;

    if (disk_getparams(disk->id, &drive_type, &cyls, &secs, &heads)) {
        return false;
    }

    disk->id = drive;
    disk->cyls = cyls;
    disk->heads = heads;
    disk->secs = secs;

    return true;
}

void lba_to_chs(DISK* disk, uint32_t lba, uint16_t* cyl_out, uint16_t* sec_out, uint16_t* head_out) {
    *sec_out = lba % disk->secs + 1;
    *cyl_out = (lba / disk->secs) / disk->heads;
    *head_out = (lba / disk->secs) % disk->heads;
}


bool read_disk_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void* data_out) {
    uint16_t cyl, sec, head;
    lba_to_chs(disk, lba, &cyl, &sec, &head);

    // 3 attempts to read
    for (int i = 0; i < 3; i++) {
        if (disk_read(disk->id, cyl, sec, head, sectors, data_out)) {
            return true;
        }

        disk_reset(disk->id);
    }

    return false;
}
