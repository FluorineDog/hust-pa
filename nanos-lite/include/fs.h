#pragma once

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif


size_t vfs_read(int fd, void* buf, int size);
size_t vfs_write(int fd, const void* buf, int size);