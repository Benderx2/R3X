#include <r3x_cpu.h>
#include <nt_malloc.h>

r3x_memory_blocks* BuildMemoryBlock(unsigned int MemSize){
	r3x_memory_blocks* memroot = nt_malloc(sizeof(r3x_memory_blocks));
	//! Allocate number of pages
	unsigned int NumberOfPages = ((((MemSize & 0xFFFFF000) + SEGMENT_SIZE) / SEGMENT_SIZE));
	memroot->NumberOfBlocks = NumberOfPages;
	memroot->MemoryBlocks = nt_malloc(NumberOfPages * sizeof(r3x_mem_block));
	for(unsigned int i = 0; i < memroot->NumberOfBlocks; i++){
		//! Mark all pages as NOEXIST
		memroot->MemoryBlocks[i].Type = RX_NOEXIST;
	}
	return memroot;
}
int MemoryMap(r3x_memory_blocks* MemBlock, RX_MM_TYPE Type, unsigned int Start, unsigned int End){
	if((Start - End % SEGMENT_SIZE) != 0){
		fprintf(stderr, "What?! Attempt to map an unaligned memory block\n");
		return -1;
	}
	unsigned int PageIndex = ((((Start & 0xFFFFF000)) / SEGMENT_SIZE));
	if(PageIndex > MemBlock->NumberOfBlocks){
		fprintf(stderr, "Attempt to map an unallocated memory block!\n");
		return -1;
	}
	MemBlock->MemoryBlocks[PageIndex].Type = Type;
	return 0;
}
int MemoryUnmap(r3x_memory_blocks* MemBlock, unsigned int Start, unsigned int End){
	if((Start - End % SEGMENT_SIZE) != 0){
		fprintf(stderr, "What?! Attempt to unmap an unaligned memory block\n");
		return -1;
	}
	unsigned int PageIndex = ((((Start & 0xFFFFF000)) / SEGMENT_SIZE));
	if(PageIndex > MemBlock->NumberOfBlocks){
		fprintf(stderr, "Attempt to unmap an unallocated memory block!\n");
		return -1;
	}
	MemBlock->MemoryBlocks[PageIndex].Type = RX_NOEXIST;
	return 0;
}
RX_MM_TYPE GetBlockTypefromAddress(r3x_memory_blocks* MemBlock, unsigned int Addr){
	unsigned int PageIndex = ((((Addr & 0xFFFFF000)) / SEGMENT_SIZE));
	if(PageIndex > MemBlock->NumberOfBlocks){
		return RX_NOEXIST;
	}
	return MemBlock->MemoryBlocks[PageIndex].Type;
}
r3x_mem_block* ReturnMemorySegment(r3x_memory_blocks* MemBlock, unsigned int Addr){
	unsigned int PageIndex = ((((Addr & 0xFFFFF000)) / SEGMENT_SIZE));
	if(PageIndex > MemBlock->NumberOfBlocks){
		return NULL;
	}
	return &MemBlock->MemoryBlocks[PageIndex];
}
