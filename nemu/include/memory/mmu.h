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

inline paddr_t get_paddr(uint32_t entry_val, uint32_t offset){
	return (entry_val & ~maskify(12)) + offset;
}

inline PDE get_pde(CR3 cr3, int id) {
	paddr_t paddr = get_paddr(cr3.val, id * sizeof(PDE));
	return (PDE) paddr_read(paddr, 4);
}

inline PTE get_pte(PDE pde, int id) {
	paddr_t paddr = get_paddr(pde.val, id * sizeof(PTE));
	return (PTE) paddr_read(paddr, 4);
}

