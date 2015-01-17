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
#include <r3x_stack.h>
#include <nt_malloc.h>
stack_construct Stack;
int PushtoStack(vstack_t* stack, int64_t object);
int64_t PopFromStack(vstack_t* stack);
int64_t GetItemS(vstack_t* stack, unsigned int idx);
vstack_t* CreateStack(void);
int Duplicate(vstack_t* stack);
int SetItemS(vstack_t* stack, unsigned int idx, int64_t i);
void DestroyStack(vstack_t* Stack);
void init_stack_construct(void)
{
	Stack.Duplicate = &Duplicate;
	Stack.GetItem = &GetItemS;
	Stack.Push = &PushtoStack;
	Stack.Pop = &PopFromStack;
	Stack.Create = &CreateStack;
	Stack.SetItem = &SetItemS;
	Stack.DestroyStack = &DestroyStack;
}
int Duplicate(vstack_t* stack)
{
	PushtoStack(stack, stack->content[stack->stack_count]);
	return 0;
}
int64_t GetItemS(vstack_t* stack, unsigned int idx)
{
	if(stack == NULL) { return -1; }
	if(stack->top_of_stack==0 || stack->stack_count == 0){ return -1; }
	if (idx > stack->stack_count){ return -1; }
	else { return (stack->content[idx]); }
}
int SetItemS(vstack_t* stack, unsigned int idx, int64_t i)
{
	if(stack == NULL) { return -1; }
	if(stack->top_of_stack==0 || stack->stack_count == 0){ return -1; }
	if (idx > stack->stack_count){ return -1; }
	else { stack->content[idx] = i; }
	return 0;
}
vstack_t* CreateStack(void)
{
	vstack_t* new_stack = nt_malloc(sizeof(vstack_t)); // Allocate stack structure
	// Set stack and other vars to null
	new_stack->content = NULL;
	new_stack->top_of_stack = 0;
	new_stack->stack_count = 0;
	new_stack->max_stack = DEFAULT_MAX_STACK_SIZE;
	return new_stack;
}
int PushtoStack(vstack_t* stack, int64_t object)
{
	// Check if we have reached the maximum value..
	if(stack->top_of_stack >= stack->max_stack){
		printf("Max stack size exceeded. Top of stack : %u, Max Stack Size : %u\n", stack->top_of_stack, stack->max_stack);
		printf("Use the -stack option to specify the number of 32-bit integers to be allocated\n");
		printf("Switching to debugger\n");
		raise(SIGSEGV);
	}
	// Check if we are going to overflow
	if (stack->stack_count == stack->top_of_stack){
		// Check if there exists a buffer
		if (stack->stack_count == 0){
			// Allocate, null out and set.
			int64_t* new_buf = NULL;
			new_buf = nt_malloc(DEFAULT_STACK_SIZE);
			stack->stack_count = 16;
			stack->content = new_buf;
		}
		else {
			// Allocate it double the size to make sure we don't run out too soon
			int64_t* new_buf = nt_malloc(stack->stack_count * 2 * sizeof(int64_t));
			// Copy the old buffer
			memcpy(new_buf, stack->content, stack->stack_count * sizeof(int64_t));
			// Free it, set the new buffer, and the new size
			nt_free(stack->content);
			stack->content = new_buf;
			stack->stack_count = stack->stack_count * 2;
		}
	}
	// finally push it.
	stack->content[stack->top_of_stack] = object;
	stack->top_of_stack++;
	return 0;
}
int64_t PopFromStack(vstack_t* stack)
{
	// if top of stack is 0, return an underflow error else decrement top of stack
	if (stack->top_of_stack == 0){
		return STACK_UFLOW_ERR;
	}
	else {
		stack->top_of_stack--;
		return stack->content[stack->top_of_stack];
	}
}
void DestroyStack(vstack_t* stack)
{
	nt_free(stack->content);
	stack->top_of_stack = 0;
	stack->stack_count = 0;
	nt_free(stack);
}
