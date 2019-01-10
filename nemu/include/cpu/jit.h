#pragma once

#include <common.h>

bool jit_exec_exec_or_open(vaddr_t cr3, vaddr_t eip);
void jit_exec_close();
