#include "device.h"
size_t serial_write(const void *buf, size_t offset, size_t len) {
    for(int i = 0; i < len; ++i) {
        _putc(((char *)buf)[i]);
    }
    return len;
}

#define NAME(key) [_KEY_##key] = #key,

static const char *keyname[256]
    __attribute__((used)) = {[_KEY_NONE] = "NONE", _KEYS(NAME)};

size_t events_read(void *buf, size_t offset, size_t len) {
    return 0;
}

static char dispinfo[128];

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    assert(offset <= 128);
    if(len > 128 - offset){
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
    if(x + len >= width){
        Log("x=%d, y=%d, len=%d", x, y, len);
    }
    // assert(x + len <= width);
    // assert(y < screen_height());
    draw_rect(buf, x, y, len, 1);
    return len * sizeof(int);
}

void init_device() {
    Log("Initializing devices...");
    _ioe_init();

    // TODO: print the string to array `dispinfo` with the format
    // described in the Navy-apps convention
    sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
    
}
