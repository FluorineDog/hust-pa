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
//	assert(addr >= 0x101c20 || addr < 0x100000);
	if (no == -1) {
		memcpy(guest_to_host(addr), &data, len);
	} else {
		mmio_write(addr, len, data, no);
	}
}

uint32_t vaddr_read(vaddr_t vaddr, int len) {
	if (!cpu.cr0.paging) {
		// no paging
		return paddr_read(vaddr, len);
	}
	if (((vaddr + len - 1) ^ vaddr) >= PAGE_SIZE){
		vaddr_t cross_base = (vaddr + len) & ~(PAGE_SIZE - 1);
		int left_len = cross_base - vaddr;
		int right_len = len - left_len;
		assert(right_len > 0);
		assert(left_len > 0);
		uint32_t dl = vaddr_read(vaddr, left_len);
		uint32_t dr = vaddr_read(cross_base, right_len);
		return dl | (dr << (left_len * 8));
//		panic("[cross boundary %08x]", vaddr);
	}
	auto paddr = extract_paddr(cpu.cr3, vaddr, false);
	return paddr_read(paddr, len);
}

void vaddr_write(vaddr_t vaddr, uint32_t data, int len) {
	if (!cpu.cr0.paging) {
		return paddr_write(vaddr, data, len);
	}
	if (((vaddr + len - 1) ^ vaddr) >= PAGE_SIZE){
		panic("[cross boundary %08x]", vaddr);
	}
	auto paddr = extract_paddr(cpu.cr3, vaddr, true);
	return paddr_write(paddr, data, len);
}

#include <unordered_map>
static std::unordered_map<uint64_t, PTE*> tlb;
paddr_t extract_paddr(CR3 cr3, vaddr_t addr_raw, bool is_write){
	using namespace MMU;
	VAddr vaddr(addr_raw);
	if(addr_raw < 0x8000000) {
		return addr_raw;
	}
	uint64_t cache_id = ((uint64_t)cr3.val << 32) | (addr_raw & ~maskify(12));
	if(auto pte_ptr = tlb[cache_id]) {
		pte_ptr->accessed = 1;
		pte_ptr->dirty |= is_write;
		return blend_paddr(pte_ptr->val, vaddr.page_offset);
	}
	auto & pde = fetch_pmem<PDE>(cr3.val, vaddr.dir_index);
	Assert(pde.present == 1, "ck, %08x->%08x", cr3.val, addr_raw);
	pde.accessed = 1;
	auto & pte = fetch_pmem<PTE>(pde.val, vaddr.table_index);
	Assert(pte.present == 1, "kc, %08x->%08x", cr3.val, addr_raw);
	pte.accessed = 1;
	pte.dirty |= is_write;
	tlb[cache_id] = &pte;
	return blend_paddr(pte.val, vaddr.page_offset);
}
