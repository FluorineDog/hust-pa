#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

typedef void (*outer_t)(char ch, struct Engine* eng);

struct OutputEngine{
    outer_t fn;
    char* buf;
    
};



static void buf_w(char ch, char* buf, size_t index, size_t maxlen){
    if(index < maxlen){
        buf[index] = ch;
    }
}

static void io_w(char ch, char* buf, size_t index, size_t maxlen){
    // TODO 
}




_vsnprintf(const char* fmt, )

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
