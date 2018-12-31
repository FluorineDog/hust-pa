/*
 * Static runtime library for a system software on AbstractMachine
 */

#ifndef __KLIB_H__
#define __KLIB_H__

#include <am.h>
#include <stdarg.h>

#define __NATIVE_USE_KLIB__

#ifdef __NATIVE_USE_KLIB__
#define strlen my_strlen
#define strcpy my_strcpy
#define strncpy my_strncpy
#define strcat my_strcat
#define strcmp my_strcmp
#define strncmp my_strncmp
#define memset my_memset
#define memcpy my_memcpy
#define memcmp my_memcmp
#define printf my_printf
#define vsprintf my_vsprintf
#define sprintf my_sprintf
#define snprintf my_snprintf
#endif

#ifdef __cplusplus
extern "C" {
#endif

// am devices

uint32_t uptime();
void get_timeofday(void* rtc);
int read_key();
void draw_rect(const uint32_t* pixels, int x, int y, int w, int h);
void draw_line(const uint32_t* pixels, int line_offset_beg, int length);
void draw_sync();
int screen_width();
int screen_height();

// string.h
void* memset(void* v, int c, size_t n);
void* memcpy(void* dst, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
size_t strlen(const char* s);
char* strcat(char* dst, const char* src);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

// stdlib.h
void srand(unsigned int seed);
int rand();

// stdio.h
int printf(const char* fmt, ...);
int sprintf(char* out, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int vsprintf(char* str, const char* format, va_list ap);

// assert.h
#ifdef NDEBUG
#define assert(ignore) ((void)sizeof(ignore))
#else
#define assert(cond)                                                 \
    do {                                                             \
        if(!(cond)) {                                                \
            printf("Assertion fail at %s:%d\n", __FILE__, __LINE__); \
            _halt(1);                                                \
        }                                                            \
    } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif
