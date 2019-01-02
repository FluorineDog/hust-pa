#include "nemu.h"
#include "device/mmio.h"
#include "memory/mmu.h"


#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	int no = is_mmio(addr);
	if (no == -1) {
		return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
	} else {
		return mmio_read(addr, len, no);
	}
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
	int no = is_mmio(addr);
	if(no == -1){
		memcpy(guest_to_host(addr), &data, len);
	} else {
		mmio_write(addr, len, data, no);
	}
}

uint32_t vaddr_read(vaddr_t vaddr, int len) {
	if(!cpu.cr0.paging){
		// no paging
		return paddr_read(vaddr, len);
	}
	auto paddr = extract_paddr(cpu.cr3, vaddr, false);
	return paddr_read(paddr, len);
}

void vaddr_write(vaddr_t vaddr, uint32_t data, int len) {
	if(!cpu.cr0.paging){
		return paddr_write(vaddr, data, len);
	}
	auto paddr = extract_paddr(cpu.cr3, vaddr, true);
	return paddr_write(paddr, data, len);
}
