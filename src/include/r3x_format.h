#ifndef __R3X_FORMAT_H
#define __R3X_FORMAT_H
#include <stdint.h>
#define R3X_HEADER_2033 0xBA5EBA11
typedef struct r3x_header {
	uint32_t header_id;
	uint32_t r3x_init;
	uint32_t r3x_text_size;
	uint32_t r3x_data;
	uint32_t r3x_data_size;
	uint32_t r3x_bss;
	uint32_t r3x_bss_size;
	uint32_t stack_size;
	uint32_t total_size;
	uint8_t name[8];
	uint8_t pulibsher[8];
} r3x_header_t;
#define PROG_EXEC_POINT 0x100000 // under 1 MB = System reserved.
uint8_t* r3x_load_executable(char* name, r3x_header_t* header);
#endif
