#ifndef REX_MEMORY_H
#define REX_MEMORY_H
#include <system.h>
//! Define bits
#define RX_MEM_NOEX_BIT 0
#define RX_MEM_NOW_BIT 1
#define RX_MEM_RONLY_BIT 2
#define RX_MEM_RW_BIT 3
typedef struct {
	uint8_t Flags;
	uint32_t StartAddress;
	uint32_t EndAddress; 
} r3x_mem_block;
typedef struct {
	r3x_mem_block** MemoryBlocks;
} r3x_memory_blocks;
#endif
