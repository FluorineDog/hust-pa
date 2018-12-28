#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#define PMEM_SIZE (128 * 1024 * 1024)
extern uint8_t pmem[];

/* convert the guest physical address in the guest program to host virtual address in NEMU */
#define guest_to_host(p) ((uint8_t *)(pmem + (unsigned)p))
/* convert the host virtual address in NEMU to guest physical address in the guest program */
#define host_to_guest(p) ((paddr_t)((uint8_t *)p - (uint8_t *)pmem))

uint32_t vaddr_read(vaddr_t, int);
uint32_t paddr_read(paddr_t, int);
void vaddr_write(vaddr_t, uint32_t, int);
void paddr_write(paddr_t, uint32_t, int);

#endif
