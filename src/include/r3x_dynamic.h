#ifndef __R3X_DYNAMIC_H
#define __R3X_DYNAMIC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <r3x_opcodes.h>
#include <r3x_cpu.h>
#include <nt_malloc.h>
#define REX_DYNAMIC_HEADER_VAL 0xDEADBEEF
typedef struct REX_DYNAMIC_HEADER {
	uint32_t header;
	uint32_t text_section;
	uint32_t text_size;
	uint32_t export_section;
	uint32_t export_size;
	uint32_t data_section;
	uint32_t data_size;
	uint32_t bss_section;
	uint32_t bss_size;
} r3x_dynamic_header_t;
typedef struct export_struct {
	uint32_t function_id;
	uint32_t instruction_pointer;
} export_struct;
typedef struct function_call_struct {
	uint32_t libid;
	uint32_t function_count;
	uint32_t loadaddr;
	export_struct* functions;
} libimport_struct;
int load_lib_manager(void);
int load_dynamic_library(char* name, r3x_cpu_t* CPU);
uint32_t dynamic_call(unsigned int libhandle, unsigned int functionhandle);
#endif
