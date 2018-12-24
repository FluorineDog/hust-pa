#pragma once
#include "proc.h"
#include "ramdisk.h"

#define DEFAULT_ENTRY 0x4000000


void naive_uload(PCB *pcb, const char *filename);
void context_uload(PCB *pcb, const char *filename);