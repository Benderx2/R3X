#ifndef __STACK_H
#define __STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define STACK_UFLOW_ERR -1
#define DEFAULT_STACK_SIZE 16 * sizeof(int)
typedef struct __stack {
	int* content;
	int top_of_stack;
	int stack_count;
} vstack_t;
typedef struct __stack_construct {
	vstack_t* (*Create)(void);
	int32_t(*Push)(vstack_t*, int32_t);
	int32_t(*Pop)(vstack_t*);
	int32_t(*Duplicate)(vstack_t*);
	int32_t(*GetItem)(vstack_t*, int);
	void(*SetItem)(vstack_t*, int, int32_t);
	void(*DestroyStack)(vstack_t*);
} stack_construct;
stack_construct Stack;
void init_stack_construct(void);
#endif
