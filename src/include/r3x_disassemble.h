#ifndef R3X_DISASSEMBLE_H
#define R3X_DISASSEMBLE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <big_endian.h>
#include <r3x_opcodes.h>
#ifdef R3X_BIG_ENDIAN 
#define BYTE_SWAP(x) BIG_ENDIAN_INT(x)
#else
#define BYTE_SWAP(x) (x)
#endif
void disassemble(uint8_t* input, unsigned int size, FILE* output, char* sectionheader);
#endif
