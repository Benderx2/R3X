/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <r3x_cpu.h>
#include <r3x_format.h>
#include <nt_malloc.h>
#include <big_endian.h>
uint32_t text_begin, text_size, data_begin, data_size;
uint8_t* r3x_load_executable(char* name, r3x_header_t* header)
{
	// open file for reading
	if(name == NULL) {
		printf("r3x_load_executable: failed to load file (Name not passed to VM), specify file using -exe <filename>\n");
		exit(EXIT_FAILURE);
	}
	FILE* fp = fopen(name, "r+b");
	if (fp == NULL)
	{
		printf("r3x_load_executable: failed to load file. (File Not Found!)\n");
		exit(EXIT_FAILURE);
	}
	// seek to end of file
	fseek(fp, 0L, SEEK_END);
	// get file size
	int size = ftell(fp);
	// allocate memory
	uint8_t* mem1 = nt_malloc(size);
	// seek reset
	fseek(fp, 0L, SEEK_SET);
	// read all bytes
	int sizeread = fread(mem1, sizeof(uint8_t), size, fp);
	if(sizeread != size) { 
		printf("fread failure. Expected %u bytes but read %u bytes\n", size, sizeread);
		exit(1);
	} 
	// assign header
	header = (r3x_header_t*)&mem1[0];
	// check if it's an executable
	#ifdef R3X_BIG_ENDIAN
	#warning "Big endian systems are fucking experimental!"
	#endif
	if(header->header_id != BIG_ENDIAN_INT(R3X_HEADER_2033))
	{
		printf("r3x_load_executable: invalid header: 0x%X.\n", header->header_id);
		nt_free(mem1);
		return NULL;
	}
	
	//! Check for page alignment for text and data sections
	if(BIG_ENDIAN_INT(header->r3x_init) % SEGMENT_SIZE != 0 || BIG_ENDIAN_INT(header->r3x_text_size) % SEGMENT_SIZE != 0 || BIG_ENDIAN_INT(header->r3x_data) % SEGMENT_SIZE != 0 || ((BIG_ENDIAN_INT(header->r3x_bss) + BIG_ENDIAN_INT(header->r3x_bss_size)) - BIG_ENDIAN_INT(header->r3x_data)) % SEGMENT_SIZE != 0){
		printf("Executable Sections not page aligned / contiguous. Corrupt or malicious executable. [BE]\n");
		printf("0x%X, 0x%X, 0x%X, 0x%X\n", BIG_ENDIAN_INT(header->r3x_init) % SEGMENT_SIZE, BIG_ENDIAN_INT(header->r3x_text_size) % SEGMENT_SIZE, BIG_ENDIAN_INT(header->r3x_data) % SEGMENT_SIZE, ((BIG_ENDIAN_INT(header->r3x_bss) + BIG_ENDIAN_INT(header->r3x_bss_size)) - BIG_ENDIAN_INT(header->r3x_data)) % SEGMENT_SIZE);
		exit(EXIT_FAILURE);
	}
	text_begin = BIG_ENDIAN_INT(header->r3x_init);
	text_size = BIG_ENDIAN_INT(header->r3x_text_size);
	data_begin = BIG_ENDIAN_INT(header->r3x_data);
	data_size = (((BIG_ENDIAN_INT(header->r3x_bss) + BIG_ENDIAN_INT(header->r3x_bss_size))-BIG_ENDIAN_INT(header->r3x_data)));
	// now get its' ACTUAL size and malloc (inclusive of bss and stack which are not included in the file)
	uint8_t* mem2 = nt_malloc(((PROG_EXEC_POINT + BIG_ENDIAN_INT(header->total_size))&0xFFFFF000)+SEGMENT_SIZE);
	memset(mem2, 0, PROG_EXEC_POINT + BIG_ENDIAN_INT(header->total_size));
	// copy the execuatable to new buffer (note: some bytes would be empty at the end as stack or bss)
	memcpy(mem2 + PROG_EXEC_POINT, mem1, size);
	// free the first buffer
	nt_free(mem1);
	fclose(fp); // Free the file no need..
	header = (r3x_header_t*)&mem2[PROG_EXEC_POINT];
	printf("Executable Name: %s\n", (char*)&mem2[BIG_ENDIAN_INT(header->nameaddr)]);
	printf("Publisher Name: %s\n", (char*)&mem2[BIG_ENDIAN_INT(header->pulibsheraddr)]); 
	// return the buffer
	return mem2;
}
void read_symbol_table(r3x_header_t* header, uint8_t* Memory, uint32_t size, uint32_t IP) {
	if(BIG_ENDIAN_INT(header->r3x_symbols) > size)
	{
		printf("Error: Corrupted symbol header! Unable to read symbol table!\n");
		return;
	} else {
		int number_of_symbols = BIG_ENDIAN_INT(header->r3x_symbolsize) / sizeof(r3x_symbol_t);
		r3x_symbol_t* mysymbols = (r3x_symbol_t*)&Memory[BIG_ENDIAN_INT(header->r3x_symbols)];
		for(int i = 0; i < number_of_symbols; i++) {
			if(BIG_ENDIAN_INT(mysymbols[i].SymbolName) > size)
			{
				printf("Name: Invalid\n");
			} else {
				printf("Name: %s\n", (char*)&Memory[BIG_ENDIAN_INT(mysymbols[i].SymbolName)]);
			}
			printf("Symbol start: 0x%X\n", BIG_ENDIAN_INT(mysymbols[i].SymbolStart));
			printf("Symbol end: 0x%X\n", BIG_ENDIAN_INT(mysymbols[i].SymbolEnd));
		}
		for(int i = 0; i <number_of_symbols; i++) {
			if(mysymbols[i].SymbolStart < IP && mysymbols[i].SymbolEnd > IP) {
					if(BIG_ENDIAN_INT(mysymbols[i].SymbolName) > size)
					{
						printf("Corrupt symbol name.\n");
						return;
					}
					else {
						printf("IP %u, belongs to symbol '%s'\n", IP, (char*)&Memory[mysymbols[i].SymbolName]);
					}
			}
		}
	}
}
