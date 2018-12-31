#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char* s) {
    int n = 0;
    while((s[n])) ++n;
    return n;
}

char* strcpy(char* dst, const char* src) {
    char* iter = dst;
    while((*iter++ = *src++)) {
    }
    return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
    char* iter = dst;
    while((n-- && (*iter++ = *src++))) {
    }
    return dst;
}

char* strcat(char* dst, const char* src) {
    char* iter = dst;
    while((*iter)) ++iter;
    while((*iter++ = *src++)) {
    }
    return dst;
}

int strcmp(const char* s1, const char* s2) {
    return strncmp(s1, s2, -1);
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for(int i = 0; i < n; ++i) {
        if(s1[i] == s2[i] && s1[i]) {
            continue;
        } else {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

void* memset(void* v, int c, size_t n) {
    for(int i = 0; i < n; ++i) {
        ((char*)v)[i] = (char)c;
    }
    return v;
}

void* memcpy(void* out, const void* in, size_t n) {
    for(int i = 0; i < n; ++i) {
        ((char*)out)[i] = ((char*)in)[i];
    }
    return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const char* left_ss = (const char*)s1;
    const char* right_ss = (const char*)s2;

    for(int i = 0; i < n; ++i) {
        char left = left_ss[i];
        char right = right_ss[i];
        if(left == right) {
            continue;
        } else {
            return left - right;
        }
    }
    return 0;
}

#endif
