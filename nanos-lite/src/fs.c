#include "fs.h"
#include "device.h"
#include "ramdisk.h"

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

typedef struct {
    char *name;
    size_t size;
    size_t disk_offset;
    ReadFn read;
    WriteFn write;
    size_t open_offset;
} Finfo;

enum { FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_DISPINFO, FD_FILES_BEGIN };

size_t invalid_read(void *buf, size_t offset, size_t len) {
    panic("should not reach here");
    return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
    panic("should not reach here");
    return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    {"stdin", 0, 0, invalid_read, invalid_write},
    {"stdout", 0, 0, invalid_read, serial_write},
    {"stderr", 0, 0, invalid_read, serial_write},
    {"/usr/fb", 0, 0, invalid_read, fb_write},
    {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
    // TODO: initialize the size of /dev/fb
    for(int i = FD_FILES_BEGIN; i < NR_FILES; ++i) {
        file_table[i].read = ramdisk_read;
        file_table[i].write = ramdisk_write;
        file_table[i].open_offset = 0;
    }
    file_table[FD_FB].size = screen_width() * screen_height() * sizeof(int);
}

size_t vfs_read(int fd, void *buf, int size) {
    assert(0 <= fd && fd < NR_FILES);
    Finfo *h = file_table + fd;
    int offset = h->open_offset + h->disk_offset;
    int delta = h->read(buf, offset, size);
    if(delta < 0) {
        panic("wtf");
        return delta;
    }
    h->open_offset += delta;
    return delta;
}

size_t vfs_write(int fd, const void *buf, int size) {
    assert(0 <= fd && fd < NR_FILES);
    Finfo *h = file_table + fd;
    int offset = h->open_offset + h->disk_offset;
    int delta = h->write(buf, offset, size);
    if(delta < 0) {
        panic("wtf");
        return delta;
    }
    h->open_offset += delta;
    return delta;
}