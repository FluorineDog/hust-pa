#include "device.h"
#include "fs.h"
size_t serial_write(const void *buf, size_t offset, size_t len) {
    // _yield();
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
    // size_t width = screen_width();
    // offset /= sizeof(int);
    // len /= sizeof(int);
    // size_t x = offset % width;
    // size_t y = offset / width;
    // if(x + len >= width) {
    //     Log("x=%d, y=%d, len=%d", x, y, len);
    // }
    // assert(x + len <= width);
    // assert(y < screen_height());
    // draw_rect(buf, x, y, len, 1);
    draw_line(buf, offset / 4, len / 4);
    return len;
}

void init_device() {
    Log("Initializing devices...");
    _ioe_init();

    int width = screen_width();
    int height = screen_height();
    int size = sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", width, height);
    vfs_set_size(FD_DISPINFO, size);
}

size_t events_read(void *buf, size_t offset, size_t len) {
    // _yield();
    // Log("buf=%p, off=%d, len=%d", buf, offset, len);
    int keycode = read_key();
    const char *status = "ku";
    if(keycode & 0x8000) {
        keycode ^= 0x8000;
        status = "kd";
        if(_KEY_F1 <= keycode && keycode <= _KEY_F3) {
            extern void set_pcb_id(int);
            set_pcb_id(keycode - _KEY_F1 + 1);
        }
    }

    static int next_time = 0;
    if(keycode != _KEY_NONE) {
        const char *name = keyname[keycode];
        // while(1);
        // printf("[%p]", buf);
        int n = snprintf(buf, len + 100, "%s %s\n", status, name);
        printf("[%s]", buf);
        assert(n < len);
        return n;
    } else {
        // time here
        int time;
        static int max_time = 0;
        if(max_time < next_time) {
            while((time = uptime()) <= next_time) {
            }
            max_time = time;
        }
        next_time++;
        // Log("[%d - %d]", next_time, max_time);
        int n = snprintf(buf, len, "t %d\n", next_time);
        return n;
    }
}
