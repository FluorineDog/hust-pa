#ifndef __MMIO_H__
#define __MMIO_H__

#include "common.h"

typedef void(*mmio_callback_t)(paddr_t paddr, int len, bool is_write);


void* add_mmio_map(paddr_t paddr, int len, mmio_callback_t callback);
int is_mmio(paddr_t paddr);

uint32_t mmio_read(paddr_t paddr, int len, int map_NO);
void mmio_write(paddr_t paddr, int len, uint32_t data, int map_NO);

#endif
