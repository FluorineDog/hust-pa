#pragma once
#include "proc.h"
#include "ramdisk.h"

#define DEFAULT_ENTRY 0x8000000

void naive_uload(PCB *pcb, const char *filename);
void context_uload(PCB *pcb, const char *filename);

void program_naive_uload(PCB *pcb, const char *filename, char *const *argv);

void context_kload(PCB *pcb, void *entry);