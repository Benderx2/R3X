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
#ifndef R3X_FORMAT_H
#define R3X_FORMAT_H
#include <stdint.h>
#define R3X_HEADER_2033 0xBA5EBA11
typedef struct r3x_header {
	uint32_t header_id;
	uint32_t program_header;
	uint16_t minor;
	uint16_t major;
	uint32_t r3x_init;
	uint32_t r3x_text_size;
	uint32_t r3x_data;
	uint32_t r3x_data_size;
	uint32_t r3x_symbols;
	uint32_t r3x_symbolsize;
	uint32_t r3x_bss;
	uint32_t r3x_bss_size;
	uint32_t stack_size;
	uint32_t total_size;
	uint32_t nameaddr;
	uint32_t pulibsheraddr;
} r3x_header_t;
typedef struct r3x_symbol {
	uint32_t SymbolName;
	uint32_t SymbolStart;
	uint32_t SymbolEnd;
} r3x_symbol_t;
#define PROG_EXEC_POINT 0x100000// under 1 MB = System reserved.
uint8_t* r3x_load_executable(char* name, r3x_header_t* header);
void read_symbol_table(r3x_header_t* header, uint8_t* Memory, uint32_t size, uint32_t IP);
#endif
