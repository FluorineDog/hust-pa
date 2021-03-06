#include "fs.h"
#include "device.h"
#include "ramdisk.h"
#include "proc.h"

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

typedef struct {
    char *name;
    size_t size;
    size_t disk_offset;
    ReadFn read;
    WriteFn write;
    ssize_t open_offset[MAX_NR_PROC];
} Finfo;

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
    {"stdin", -1, 0, invalid_read, invalid_write},
    {"stdout", -1, 0, invalid_read, serial_write},
    {"stderr", -1, 0, invalid_read, serial_write},
    {"/dev/fb", 0, 0, invalid_read, fb_write},
    {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
    {"/dev/events", -1, 0, events_read, invalid_write},
    {"/dev/tty", -1, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
    for(int i = FD_FILES_BEGIN; i < NR_FILES; ++i) {
        file_table[i].read = ramdisk_read;
        file_table[i].write = ramdisk_write;
    }
    file_table[FD_FB].size = screen_width() * screen_height() * sizeof(int);
}

size_t vfs_read(int fd, void *buf, size_t size) {
    assert(0 <= fd && fd < NR_FILES);
    Finfo *h = file_table + fd;
    int offset = h->open_offset[get_pcb_id()] + h->disk_offset;
    int remaining = h->size - h->open_offset[get_pcb_id()];
    if(size > remaining) {
        size = remaining;
    }
    int delta = h->read(buf, offset, size);
    assert(size == delta || h->size == (size_t)-1);
    if(delta < 0) {
        panic("wtf");
        return delta;
    }
    // Log("read{fd = %d, buf=%p, size=%d} from %d to %d", fd, buf, h->size, h->open_offset[get_pcb_id()],
    //     h->open_offset[get_pcb_id()] + delta);
    h->open_offset[get_pcb_id()] += delta;
    return delta;
}

size_t vfs_write(int fd, const void *buf, size_t size) {
    assert(0 <= fd && fd < NR_FILES);
    Finfo *h = file_table + fd;
    int offset = h->open_offset[get_pcb_id()] + h->disk_offset;
    int delta = h->write(buf, offset, size);
    if(delta < 0) {
        panic("wtf");
        return delta;
    }
    if(size != delta) {
        Log("write %d from %d to %d[%d]", fd, h->open_offset[get_pcb_id()], h->open_offset[get_pcb_id()] + delta,
            size);
    }
    assert(size == delta);
    h->open_offset[get_pcb_id()] += delta;
    assert(h->open_offset[get_pcb_id()] <= h->size);
    return delta;
}

// @ret: fd
int vfs_open(const char *filename, int flags, int mode) {
    Log("opening %s", filename);
    // assert(0);
    for(int fd = 0; fd < NR_FILES; ++fd) {
        Finfo *handle = file_table + fd;
        if(strcmp(handle->name, filename) == 0) {
            // match
            // assert(handle->file_lock == 0);
            // handle->file_lock++;
            handle->open_offset[get_pcb_id()] = 0;
            Log("opened with fd=%d", fd);
            return fd;
        }
    }
    return -1;
}

size_t vfs_filesz(int fd) {
    Log("get size of %d", fd);
    Finfo *handle = file_table + fd;
    return handle->size;
}

int vfs_close(int fd) {
    // Finfo *handle = file_table + fd;
    // assert(handle->file_lock == 1);
    // handle->file_lock--;
    Log("closing %d", fd);
    return 0;
}

ssize_t vfs_lseek(int fd, ssize_t offset, int whence) {
    Finfo *handle = file_table + fd;
    ssize_t base;
    switch(whence) {
        case SEEK_SET: base = 0; break;
        case SEEK_CUR: base = handle->open_offset[get_pcb_id()]; break;
        case SEEK_END: base = handle->size; break;
        default: panic("wtf");
    }
    ssize_t new = base + offset;
    // Log("lseek %d{size = %d, off=%d} with {offset=%d, whence=%d}, to %d", fd,
    //     handle->size, handle->open_offset[get_pcb_id()], offset, whence, new);
    assert(0 <= new);
    // piss off the fix size
    assert(new <= handle->size);
    handle->open_offset[get_pcb_id()] = new;
    return new;
}

void vfs_set_size(int fd, size_t size) {
    assert(0 <= size);
    assert(0 <= fd && fd < FD_FILES_BEGIN);
    file_table[fd].size = size;
}