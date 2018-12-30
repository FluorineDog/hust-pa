#include "device.h"
#include "fs.h"
size_t serial_write(const void *buf, size_t offset, size_t len) {
    for(int i = 0; i < len; ++i) {
        _putc(((char *)buf)[i]);
    }
    return len;
}

#define NAME(key) [_KEY_##key] = #key,

static const char *keyname[256] = {[_KEY_NONE] = "NONE", _KEYS(NAME)};

static char dispinfo[128];

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    Log("display");
    assert(offset <= 128);
    if(len > 128 - offset) {
        len = 128 - offset;
    }
    memcpy(buf, dispinfo + offset, len);
    return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    // TODO();
    size_t width = screen_width();
    offset /= sizeof(int);
    len /= sizeof(int);
    size_t x = offset % width;
    size_t y = offset / width;
    if(x + len >= width) {
        Log("x=%d, y=%d, len=%d", x, y, len);
    }
    assert(x + len <= width);
    assert(y < screen_height());
    draw_rect(buf, x, y, len, 1);
    return len * sizeof(int);
}

void init_device() {
    Log("Initializing devices...");
    _ioe_init();

    // TODO: print the string to array `dispinfo` with the format
    // described in the Navy-apps convention
    int width = screen_width();
    int height = screen_height();
    int size = sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", width, height);
    vfs_set_size(FD_DISPINFO, size);
}

size_t events_read(void *buf, size_t offset, size_t len) {
    // Log("buf=%p, off=%d, len=%d", buf, offset, len);
    int keycode = read_key();
    const char *status = "ku";
    if(keycode & 0x8000) {
        keycode ^= 0x8000;
        status = "kd";
    }

    static int next_time = 0;
    if(keycode != _KEY_NONE) {
        const char *name = keyname[keycode];
        int n = snprintf(buf, len + 100, "%s %s\n", status, name);
        assert(n < len);
        return n;
    } else {
        // time here
        int time;
        static int max_time = -1;
        if(next_time < max_time) {
            while((time = uptime()) <= next_time) {
            }
            max_time = time;
        }
        next_time++;
        int n = snprintf(buf, len, "t %d\n", next_time);
        return n;
    }
}