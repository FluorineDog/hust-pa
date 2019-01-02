#pragma once

#include <stdint.h>

/* 32bit x86 uses 4KB page size */
#define PAGE_SIZE 4096
#define NR_PDE 1024
#define NR_PTE 1024
#define PAGE_MASK (4096 - 1)
#define PT_SIZE ((NR_PTE) * (PAGE_SIZE))

/* the Control Register 0 */
typedef union CR0 {
	struct {
		uint32_t protect_enable : 1;
		uint32_t dont_care : 30;
		uint32_t paging : 1;
	};
	uint32_t val;
} CR0;

/* the Control Register 3 (physical address of page directory) */
typedef union {
	struct {
		uint32_t pad0 : 12;
		uint32_t page_directory_base : 20;
	};
	uint32_t val;
} CR3;


/* the 32bit Page Directory(first level page table) data structure */
typedef union PageDirectoryEntry {
	PageDirectoryEntry(uint32_t val) : val(val) {}
	struct {
		uint32_t present : 1;
		uint32_t read_write : 1;
		uint32_t user_supervisor : 1;
		uint32_t page_write_through : 1;
		uint32_t page_cache_disable : 1;
		uint32_t accessed : 1;
		uint32_t pad0 : 6;
		uint32_t page_frame : 20;
	};
	uint32_t val;
} PDE;

/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	PageTableEntry(uint32_t val): val(val){}
	struct {
		uint32_t present : 1;
		uint32_t read_write : 1;
		uint32_t user_supervisor : 1;
		uint32_t page_write_through : 1;
		uint32_t page_cache_disable : 1;
		uint32_t accessed : 1;
		uint32_t dirty : 1;
		uint32_t pad0 : 1;
		uint32_t global : 1;
		uint32_t pad1 : 3;
		uint32_t page_frame : 20;
	};
	uint32_t val;
} PTE;

typedef PTE (*PT)[NR_PTE];
typedef PDE (*PD)[NR_PDE];

typedef union GateDescriptor {
	struct {
		uint32_t offset_15_0 : 16;
		uint32_t dont_care0 : 16;
		uint32_t dont_care1 : 15;
		uint32_t present : 1;
		uint32_t offset_31_16 : 16;
	};
	uint32_t val;
} GateDesc;

namespace MMU {
union VAddr{
	VAddr(uint32_t val): val(val) {}
	struct {
		uint32_t page_offset : 12;
		uint32_t table_index : 10;
		uint32_t dir_index : 10;
	};
	uint32_t val;
};

inline paddr_t blend_paddr(uint32_t base_addr, int offset){
	return (base_addr & ~maskify(12)) + offset;
}

template<typename Type>
inline Type& fetch_pmem(uint32_t base_addr, int index){
	auto paddr = blend_paddr(base_addr, index * sizeof(Type));
	return (Type&)pmem[paddr];
}

//inline PDE get_pde(CR3 cr3, int index) {
//	paddr_t pde_paddr = blend_paddr(cr3.val, index * sizeof(PDE));
//	return (PDE) paddr_read(pde_paddr, 4);
//}
//
//inline PTE get_pte(PDE pde, int index) {
//	paddr_t pte_paddr = blend_paddr(pde.val, index * sizeof(PTE));
//	return (PTE) paddr_read(pte_paddr, 4);
//}

inline paddr_t extract_paddr(CR3 cr3, vaddr_t addr_raw, bool is_write){
	VAddr vaddr(addr_raw);
	printflog("accessing %08x", addr_raw);
	auto & pde = fetch_pmem<PDE>(cr3.val, vaddr.dir_index);
	assert(pde.present == 1);
	pde.accessed = 1;
	auto & pte = fetch_pmem<PTE>(pde.val, vaddr.dir_index);
	assert(pte.present == 1);
	pte.accessed = 1;
	pte.dirty |= is_write;
	return blend_paddr(pte.val, vaddr.page_offset);
}

}

using MMU::extract_paddr;
