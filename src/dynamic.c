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
// REX Virtual Dynamic Linker
// Note: This is different from the native dynamic linker, it is meant to dynamically link 2 REX bytecodes.
#include <r3x_dynamic.h>
#include <big_endian.h>
libimport_struct** lbstructs;
unsigned int total_number_of_structs = 16;
unsigned int number_of_used_structs = 0;
int load_lib_manager(void) { 
	lbstructs = nt_malloc(sizeof(libimport_struct*)*16);
	return 0;
}
int load_dynamic_library(char* name, r3x_cpu_t* CPU) {
	uint32_t loadaddr = CPU->MemorySize;
	FILE* dynamic_lib_file = fopen(name, "r");
	if(dynamic_lib_file == NULL) {	
		printf("Unable to load dynamic object file: %s. ABORTING..\n", name);
		exit(0);
	}
	fseek(dynamic_lib_file, 0L, SEEK_END);
	unsigned int totalsize = ftell(dynamic_lib_file);
	fseek(dynamic_lib_file, 0L, SEEK_SET);
	uint8_t* temp = nt_malloc(totalsize * sizeof(uint8_t));
	unsigned int sizeread = fread(temp, sizeof(uint8_t), totalsize, dynamic_lib_file);
	if(sizeread != totalsize) { 
		printf("Unable to read dynamic object file properly. ABORTING..\n");
		exit(0);
	}
	r3x_dynamic_header_t* dyn_header = (r3x_dynamic_header_t*)&(temp[0]);
	if(BIG_ENDIAN_INT(dyn_header->header) != REX_DYNAMIC_HEADER_VAL)
	{
		printf("Invalid dynamic object file. ABORTING..\n");
		exit(EXIT_FAILURE);
	}
	if(BIG_ENDIAN_INT(dyn_header->text_section) % SEGMENT_SIZE != 0 || BIG_ENDIAN_INT(dyn_header->text_size) % SEGMENT_SIZE != 0 || BIG_ENDIAN_INT(dyn_header->data_section) % SEGMENT_SIZE != 0 || (BIG_ENDIAN_INT(dyn_header->bss_section) + BIG_ENDIAN_INT(dyn_header->bss_size) - BIG_ENDIAN_INT(dyn_header->data_section)) % SEGMENT_SIZE != 0){
		printf("Attempt to load a malicious dynamic library. Executable sections not page aligned.\n");
		printf("0x%X, 0x%X, 0x%X, 0x%X\n", BIG_ENDIAN_INT(dyn_header->text_section) % SEGMENT_SIZE, BIG_ENDIAN_INT(dyn_header->text_size) % SEGMENT_SIZE, BIG_ENDIAN_INT(dyn_header->data_section) % SEGMENT_SIZE, (BIG_ENDIAN_INT(dyn_header->bss_section) + BIG_ENDIAN_INT(dyn_header->bss_size) - BIG_ENDIAN_INT(dyn_header->data_section)) % SEGMENT_SIZE);
		exit(EXIT_FAILURE);
	}
	if(number_of_used_structs >= total_number_of_structs) { 
		lbstructs = nt_realloc(lbstructs, sizeof(libimport_struct*)*(total_number_of_structs+16));
		total_number_of_structs += 16;
	}
	uint32_t begin_text = loadaddr + BIG_ENDIAN_INT(dyn_header->text_section);
	uint32_t end_text = loadaddr + BIG_ENDIAN_INT(dyn_header->text_section) + BIG_ENDIAN_INT(dyn_header->text_size);
	uint32_t begin_data = loadaddr + BIG_ENDIAN_INT(dyn_header->data_section);
	uint32_t end_data = loadaddr + (BIG_ENDIAN_INT(dyn_header->bss_section) + BIG_ENDIAN_INT(dyn_header->bss_size));
	totalsize =  (totalsize & 0xFFFFF000)+SEGMENT_SIZE;
	CPU->Memory = nt_realloc(CPU->Memory, CPU->MemorySize+totalsize);
	CPU->CPUMemoryBlocks = RebuildMemoryBlock(CPU->CPUMemoryBlocks, CPU->MemorySize+totalsize);
	MemoryMap(CPU->CPUMemoryBlocks, RX_EXEC, begin_text, end_text);
	MemoryMap(CPU->CPUMemoryBlocks, RX_RW, begin_data, end_data);
	memcpy(&CPU->Memory[CPU->MemorySize], temp, totalsize);
	nt_free(temp);
	for(unsigned int i = 0; i < total_number_of_structs; i++) { 
		if(lbstructs[i] == NULL)  {
			lbstructs[i] = nt_malloc(sizeof(libimport_struct));
			lbstructs[i]->loadaddr = loadaddr;
			dyn_header = (r3x_dynamic_header_t*)&CPU->Memory[loadaddr];
			lbstructs[i]->libid = i;
			lbstructs[i]->function_count = BIG_ENDIAN_INT(dyn_header->export_size) / sizeof(export_struct);
			lbstructs[i]->functions = (export_struct*)&CPU->Memory[loadaddr + BIG_ENDIAN_INT(dyn_header->export_section)];
			CPU->MemorySize += totalsize;
			return i;
		}
	}
	return -1;
}
uint32_t dynamic_call(r3x_cpu_t* CPU, unsigned int libhandle, char* functionhandle) { 
	if(total_number_of_structs <= libhandle) {
		return 0;
	} else {
		if(lbstructs[libhandle] != NULL) {
			for(unsigned int i = 0; i < lbstructs[libhandle]->function_count; i++) {
				/**!
				 * Sometimes when I look at this shit, I just want to cry at the fact that
				 * humanity has gone too far.
				 * fkn.
				 * 
				 * -Regards,
				 * Benderx2
				 * 
				 * Credits:
				 * 666
				 * 69
				 * 6969
				 * 666+69
				 * 666 = 6*3 = 18 / 3 = 3?!!?!?! illuminate cu*tfirmed?
				 * 69 + 666 = 735 = 7+3+5 = 15 = 15 / 3 = 5 - 2 = 3. woah
				 * Also,
				 * (3+3)/2 = 3 = (3+3)/2 = 3...... . Illuminati supports recursion too.
				 * 
				 * take a random number, multiply it by 3, then by 4, then add 13 to it.
				 * divide the number by 12 (illuminati 4x = 12), then subtract the remainder off,
				 * from the quotient. You'll get the same number.
				 * 
				 * #mathzskillz #illuminatemath #fknidontknowshit #everything
				 * 
				 * I'm sorry if the above wasn't too scary to scare you off
				 * It's designed as a scarecrow to prevent people from reading ahead.
				 * 
				 * 
				 * 
				 * 
				 * .........but the below definitely will.
				**/
				if(!strcmp(functionhandle, (char*)((uintptr_t)((uintptr_t)(CPU->Memory) + lbstructs[libhandle]->loadaddr + BIG_ENDIAN_INT(lbstructs[libhandle]->functions[i].function_id))))) {
					/**! Damn, son! Where'd you find this? **/
					/**! Answer: Deep inside my heart, I know what I was capable of. I knew the possibilities, of what
					 * I could acheive, I knew my destiny, and today I've found it. I've done what no C programmer could
					 * do! I HAVE WRITTEN SHIT C CODE YOU SON OF A DOG. [no 'bitch' because it's considered unethical
					 * as per today's standards to use such deregatory words against a lady. hence i thought it'd be more
					 * appropriate to just tell shit about your dad, rather than your mom, because it's sexist and patriachial.]
					**/
					return lbstructs[libhandle]->loadaddr + lbstructs[libhandle]->functions[i].instruction_pointer;
				}
				/**!
				 * CALL 911 IMMEDIATELY PLZ.
				**/
			}
		}
	}
	return 0;
}
uint32_t return_dynamic_load_addr(unsigned int libhandle) {
	if(total_number_of_structs <= libhandle) {
		return 0;
	}
	if(lbstructs[libhandle] != NULL) {
		return lbstructs[libhandle]->loadaddr;
	}
	return 0;
}
