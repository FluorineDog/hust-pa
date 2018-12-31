#include "ramdisk.h"

/* The kernel is monolithic, therefore we do not need to
 * translate the address `buf' from the user process to
 * a physical one, which is necessary for a microkernel.
 */

/* read `len' bytes starting from `offset' of ramdisk into `buf' */
size_t ramdisk_read(void *buf, size_t offset, size_t len) {
    assert(offset + len <= RAMDISK_SIZE);
    // Log("CP %p INTO %p size=%d", &ramdisk_start + offset, buf, len);
    memcpy(buf, &ramdisk_start + offset, len);
    // Log("CPed");
    return len;
}

/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
size_t ramdisk_write(const void *buf, size_t offset, size_t len) {
    assert(offset + len <= RAMDISK_SIZE);
    memcpy(&ramdisk_start + offset, buf, len);
    return len;
}

void init_ramdisk() {
    Log("ramdisk info: start = %p, end = %p, size = %d bytes", &ramdisk_start,
        &ramdisk_end, (int)RAMDISK_SIZE);
}

size_t get_ramdisk_size() {
    return RAMDISK_SIZE;
}
