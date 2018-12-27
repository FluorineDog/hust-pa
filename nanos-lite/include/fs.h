#pragma once

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif


size_t vfs_read(int fd, void* buf, size_t size);
size_t vfs_write(int fd, const void* buf, size_t size);
int vfs_open(const char* filename, int flags, int mode);
size_t vfs_filesz(int fd);
int vfs_close(int fd);

ssize_t vfs_lseek(int fd, ssize_t offset, int whence);

void vfs_set_size(int fd, size_t size);
enum { FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_DISPINFO, FD_EVENTS, FD_FILES_BEGIN };