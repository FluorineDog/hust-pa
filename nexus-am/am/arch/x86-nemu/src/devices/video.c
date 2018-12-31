#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <x86.h>

#define SCREEN_PORT 0x100

static uint32_t *const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
    switch(reg) {
        case _DEVREG_VIDEO_INFO: {
            _VideoInfoReg *info = (_VideoInfoReg *)buf;
            uint32_t data = inl(SCREEN_PORT);
            info->width = data >> 16;
            info->height = data & 0xFFFF;
            return sizeof(_VideoInfoReg);
        }
    }
    return 0;
}

size_t video_write(uintptr_t reg, const void *buf, size_t size) {
    switch(reg) {
        case _DEVREG_VIDEO_FBCTL: {
            _FBCtlReg *ctl = (_FBCtlReg *)buf;

            // int size = screen_width() * screen_height();
            int W = screen_width();
            uint32_t *base = fb + ctl->y * W + ctl->x;
            for(int dy = 0; dy < ctl->h; ++dy) {
                for(int dx = 0; dx < ctl->w; ++dx) {
                    base[dy * W + dx] = ctl->pixels[dy * ctl->w + dx];
                }
            }
            if(ctl->sync) {
                // do nothing, hardware syncs.
            }
            return sizeof(_FBCtlReg);
        }
        case _DEVREG_VIDEO_FBCTL_LINE: {
            _FBCtlReg *ctl = (_FBCtlReg *)buf;
            // int size = screen_width() * screen_height();
            uint32_t *base = fb + ctl->line_offset_beg;
            for(int d = 0; d < ctl->length; ++d) {
                base[d] = ctl->pixels[d];
            }
            if(ctl->sync) {
                // do nothing, hardware syncs.
            }
            return sizeof(_FBCtlReg);
        }
    }
    return 0;
}

void vga_init() {}
