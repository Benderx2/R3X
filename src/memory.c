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
		memroot->MemoryBlocks[i].MemorySegment = i * SEGMENT_SIZE;
	}
	return memroot;
}
r3x_memory_blocks* RebuildMemoryBlock(r3x_memory_blocks* OldMemBlock, unsigned int MemSize){
	r3x_memory_blocks* new_memroot = nt_malloc(sizeof(r3x_memory_blocks));
	//! Allocate number of pages
	unsigned int NumberOfPages = ((((MemSize & 0xFFFFF000) + SEGMENT_SIZE) / SEGMENT_SIZE));
	new_memroot->NumberOfBlocks = NumberOfPages;
	new_memroot->MemoryBlocks = nt_malloc(NumberOfPages * sizeof(r3x_mem_block));
	for(unsigned int i = 0; i < OldMemBlock->NumberOfBlocks; i++){
		new_memroot->MemoryBlocks[i].Type = RX_NOEXIST;
		new_memroot->MemoryBlocks[i].MemorySegment = i * SEGMENT_SIZE;
	}
	for(unsigned int i = 0; i < OldMemBlock->NumberOfBlocks; i++){
		if(i >= NumberOfPages){
			break;
		} else {
			new_memroot->MemoryBlocks[i].Type = OldMemBlock->MemoryBlocks[i].Type;
			new_memroot->MemoryBlocks[i].MemorySegment = OldMemBlock->MemoryBlocks[i].MemorySegment;
		}
	}
	nt_free(OldMemBlock->MemoryBlocks);
	nt_free(OldMemBlock);
	return new_memroot;
}
int MemoryMap(r3x_memory_blocks* MemBlock, RX_MM_TYPE Type, unsigned int Start, unsigned int End){
	if(((End - Start) % SEGMENT_SIZE) != 0 || End % SEGMENT_SIZE != 0 || Start % SEGMENT_SIZE != 0){
		fprintf(stderr, "Warning: Attempt to map an unaligned memory block(s). Address (Start): 0x%X, Address(End): 0x%X\n", Start, End);		
	}
	unsigned int PageIndex = ((((Start & 0xFFFFF000)) / SEGMENT_SIZE));
	unsigned int NumberOfPagestoMap, EndDivisor;
	if(End % SEGMENT_SIZE == 0){
		EndDivisor = End+SEGMENT_SIZE;
	} else {
		EndDivisor = (((End & 0xFFFFF000)+SEGMENT_SIZE));
	}
	if(End - Start == SEGMENT_SIZE){
		NumberOfPagestoMap = 1;
	} else {
		NumberOfPagestoMap = ((EndDivisor - (Start & 0xFFFFF000)) / (SEGMENT_SIZE));
	}
	if(PageIndex > MemBlock->NumberOfBlocks || PageIndex + NumberOfPagestoMap > MemBlock->NumberOfBlocks){
		fprintf(stderr, "Attempt to map an unallocated memory block!\n");
		printf("Starting Address of current map: 0x%X, End Address of current map: 0x%X\n", Start, End);
		printf("Starting page of current map: %u, Number Of Pages to be mapped : %u, Total Number of available pages: %u\n", PageIndex, NumberOfPagestoMap, MemBlock->NumberOfBlocks);
		return -1;
	}
	for(unsigned int i = 0; i <= NumberOfPagestoMap; i++){
		MemBlock->MemoryBlocks[PageIndex+i].Type = Type;
	}
	return 0;
}
int DumpMemoryMap(r3x_memory_blocks* MemBlock){
	for(unsigned int i = 0; i < MemBlock->NumberOfBlocks; i++){
		printf("Page: %u, Memory range: 0x%X to 0x%X, marked as: ", i, i * SEGMENT_SIZE, (i * SEGMENT_SIZE) + SEGMENT_SIZE);
		switch(MemBlock->MemoryBlocks[i].Type){
			case RX_EXEC:
				printf("<Execute>\n");
				break;
			case RX_RONLY:
				printf("<Read-Only>\n");
				break;
			case RX_RW:
				printf("<Read/Write>\n");
				break;
			default:
				printf("<Hidden (Not accessible)>\n");
				break;
		}
	}
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
r3x_mem_block* ReturnMemorySegment(r3x_memory_blocks* MemBlock, unsigned int Addr){
	unsigned int PageIndex = ((((Addr & 0xFFFFF000)) / SEGMENT_SIZE));
	if(PageIndex > MemBlock->NumberOfBlocks){
		return NULL;
	}
	return &MemBlock->MemoryBlocks[PageIndex];
}
