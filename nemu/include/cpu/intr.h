#pragma once
#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr);
void return_from_intr();

bool dev_raise_intr();
