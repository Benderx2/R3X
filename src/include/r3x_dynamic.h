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
#ifndef R3X_DYNAMIC_H
#define R3X_DYNAMIC_H
#include <system.h>
#include <r3x_opcodes.h>
#include <r3x_cpu.h>
#include <nt_malloc.h>
#define REX_DYNAMIC_HEADER_VAL 0xDEADBEEF
typedef struct REX_DYNAMIC_HEADER {
	uint32_t header;
	uint32_t minor;
	uint32_t major;
	uint32_t text_section;
	uint32_t text_size;
	uint32_t export_section;
	uint32_t export_size;
	uint32_t data_section;
	uint32_t data_size;
	uint32_t bss_section;
	uint32_t bss_size;
	uint32_t nameaddr;
	uint32_t publisheraddr;
} r3x_dynamic_header_t;
typedef struct export_struct {
	uint32_t function_id;
	uint32_t instruction_pointer;
} export_struct;
typedef struct function_call_struct {
	char* Name;
	uint32_t libid;
	uint32_t function_count;
	uint32_t loadaddr;
	export_struct* functions;
} libimport_struct;

int load_lib_manager(void);

void load_dependencies(r3x_cpu_t* CPU);

int load_dynamic_library(char* name, r3x_cpu_t* CPU);

uint32_t dynamic_call(r3x_cpu_t* CPU, unsigned int libhandle, char* functionhandle);

uint32_t return_dynamic_load_addr(unsigned int libhandle);
#endif
