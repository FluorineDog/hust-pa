#include <stdarg.h>
#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// typedef void (*outer_t)(struct OutputEngine* eng);

typedef struct OutputEngine {
    char ch;    // mutable
    int index;    // mutable
    void (*fn)(struct OutputEngine*);
    char* buf;
    int maxlen;
} OutputEngine;

static void buf_w(OutputEngine* eng) {
    char ch = eng->ch;
    int index = eng->index;
    int maxlen = eng->maxlen;
    if(index < maxlen - 1) {
        eng->buf[index] = ch;
    } else if(index == maxlen - 1){
        eng->buf[index] = '\0';
    }
}

static void io_w(OutputEngine* eng) {
    // TODO
    // char ch = eng->ch;
    // (char)ch;
}

static void exec(OutputEngine* eng, char ch, int index) {
    eng->ch = ch;
    eng->index = index;
    eng->fn(eng);
}

int atoi_internal(char* buf, int x_) {
    int prefix = 0;
    unsigned x = x_;
    if(x < 0) {
        buf[0] = '-';
        x = -x_;
        prefix = 1;
    } else if(x == 0) {
        buf[0] = '0';
        return 1;
    }
    int idx = prefix; 
    while(x > 0){
       buf[idx] = x % 10 + '0';
       x /= 10;
       ++idx;
    }
    int iter1 = prefix; 
    int iter2 = idx-1;
    while(iter1 < iter2){
        char tmp = buf[iter1];
        buf[iter1] = buf[iter2];
        buf[iter2] = tmp;
        ++iter1;
        --iter2;
    }
    return idx;
}

int handler(OutputEngine* eng, const char* fmt, va_list va) {
    int dest_idx = 0;
    while(*fmt) {
        if(*fmt != '%') {
            exec(eng, *fmt, dest_idx++);
            ++fmt;
            continue;
        }
        assert(*fmt == '%');
        ++fmt;
        switch(*fmt) { 
            case 'd': {
                int x = va_arg(va, int);
                char buf[30];
                const int delta = atoi_internal(buf, x);
                for(int i = 0; i < delta; ++i){
                    exec(eng, buf[i], dest_idx++);
                }
                ++fmt;
                continue;
            }
            case 's': {
                const char* str = va_arg(va, char*);
                while(*str) {
                    exec(eng, *str++, dest_idx++);
                }
                ++fmt;
                continue;
            }
            default: goto FUCK;
        }
    }
    exec(eng, 0, dest_idx++);
FUCK:
    return dest_idx - 1;
}


int printf(const char* fmt, ...) {
    OutputEngine eng;
    eng.fn = io_w;
    va_list va; 
    va_start(va, fmt);
    int ref = handler(&eng, fmt, va);
    va_end(va);
    return ref;
}

int vsprintf(char* out, const char* fmt, va_list va) {
    OutputEngine eng;
    eng.buf = out;
    eng.maxlen = 1 << 30;
    eng.fn = buf_w;
    return handler(&eng, fmt, va);
}

int sprintf(char* out, const char* fmt, ...) {
    OutputEngine eng;
    eng.fn = buf_w;
    eng.buf = out;
    eng.maxlen = 1 << 30;
    va_list va; 
    va_start(va, fmt);
    int ref = handler(&eng, fmt, va);
    va_end(va);
    return ref;
}

int snprintf(char* out, size_t n, const char* fmt, ...) {
    OutputEngine eng;
    eng.fn = buf_w;
    eng.buf = out;
    eng.maxlen = n;
    va_list va; 
    va_start(va, fmt);
    int ref = handler(&eng, fmt, va);
    va_end(va);
    return ref;
}

#endif
