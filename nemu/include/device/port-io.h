#pragma once
#include "common.h"

typedef void(*pio_callback_t)(ioaddr_t, int, bool);

void* add_pio_map(ioaddr_t, int, pio_callback_t);

uint32_t pio_read_common(ioaddr_t addr, int len);
void pio_write_common(ioaddr_t addr, uint32_t data, int len);

