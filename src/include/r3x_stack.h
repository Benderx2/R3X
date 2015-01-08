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
#ifndef R3X_VSTACK_H
#define R3X_VSTACK_H
#include <system.h>
#define STACK_UFLOW_ERR -1
#define DEFAULT_STACK_SIZE 16 * sizeof(int32_t)
#define DEFAULT_MAX_STACK_SIZE 262144
typedef struct __stack {
	int32_t* content;
	unsigned int top_of_stack;
	unsigned int stack_count;
	unsigned int max_stack; 
} vstack_t;
typedef struct __stack_construct {
	vstack_t* (*Create)(void);
	int32_t(*Push)(vstack_t*, int32_t);
	int32_t(*Pop)(vstack_t*);
	int32_t(*Duplicate)(vstack_t*);
	int32_t(*GetItem)(vstack_t*, unsigned int);
	int(*SetItem)(vstack_t*, unsigned int, int32_t);
	void(*DestroyStack)(vstack_t*);
} stack_construct;
stack_construct Stack;
void init_stack_construct(void);
#endif
