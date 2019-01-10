#pragma once

#include <common.h>
#include <optional>

namespace jit {
std::optional<int> exec_exec_or_open(vaddr_t cr3, vaddr_t eip);
void inst_barrier();
void exec_close();
}
