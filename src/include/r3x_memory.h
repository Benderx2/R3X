#ifndef REX_MEMORY_H
#define REX_MEMORY_H
#include <system.h>
#define SEGMENT_SIZE 4*1024
typedef enum {
	//! Execute?
	RX_NOEXEC,
	//! Read Only?
	RX_RONLY,
	//! Readable/Writeable
	RX_RW,
	//! Doesn't exist.
	RX_NOEXIST
} RX_MM_TYPE;
typedef struct {
	RX_MM_TYPE Type;
	uint32_t MemorySegment;
} r3x_mem_block;
typedef struct {
	r3x_mem_block* MemoryBlocks;
	uint32_t NumberOfBlocks;
} r3x_memory_blocks;
r3x_memory_blocks* BuildMemoryBlock(unsigned int);
int MemoryMap(r3x_memory_blocks*, RX_MM_TYPE, unsigned int, unsigned int);
int MemoryUnmap(r3x_memory_blocks*, unsigned int, unsigned int);
RX_MM_TYPE GetBlockTypefromAddress(r3x_memory_blocks*, unsigned int);
r3x_mem_block* ReturnMemorySegment(r3x_memory_blocks*, unsigned int);
#endif
