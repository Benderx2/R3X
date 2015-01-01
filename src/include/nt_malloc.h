#ifndef __NT_MALLOC_H
#define __NT_MALLOC_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
extern bool isAllFreed;
int nt_malloc_init(bool);
void* nt_calloc(unsigned int, size_t);
void* nt_realloc(void*, size_t);
void* nt_malloc(size_t);
int nt_free(void*);
void nt_freeall(void);
void nt_atexit(void);
char* nt_concat(char *s1, char *s2);
#endif
