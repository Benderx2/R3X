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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <r3x_format.h>
#include <r3x_cpu.h>
#include <r3x_opcodes.h>
#include <r3x_stack.h>
#include <r3x_native.h>
#include <r3x_dynamic.h>
#include <r3x_stream.h>
#include <r3x_exception.h>
#include <nt_malloc.h>
#include <big_endian.h>
#ifdef REX_GRAPHICS
#include <r3x_graphics.h>
#endif
// Unions to make life easier
typedef union __32bit_typecast {
	uint32_t __num32;
	struct {
		#ifdef R3X_LITTLE_ENDIAN
		uint8_t a, b, c, d;
		#endif
		#ifdef R3X_BIG_ENDIAN
		uint8_t d, c, b, a;
	 	#endif
	} __num8;
} __32bit_typecast;

typedef union __float_typecast {
	uint32_t __num32;
	float float32;
} __float_typecast;

// A few constants
const int SLEEP_MILLISECONDS = 1000;

// Bools and global stuff
bool exitcalled = false;
bool is_read = true;
char keycode = 0;
int code = 0;
SDL_Event key_event = {SDL_USEREVENT};

// Helpful macros
#define get_item_from_stack_top(x) Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-x)
#define cpu_sleep(time, unit) usleep(time*unit)


// Rotate ints
#define  ROL_C(num, bits) ((uint32_t)num << (uint32_t)bits) | ((uint32_t)num >> (32 - (uint32_t)bits))
#define  ROR_C(num, bits) ((uint32_t)num >> (uint32_t)bits) | ((uint32_t)num << (32 - (uint32_t)bits))


// <type>-to-uint32 helpers
uint32_t return_32bit_int(uint8_t, uint8_t, uint8_t, uint8_t);
uint32_t return_32bit_int_from_ip(r3x_cpu_t*);
float return_float(uint32_t);
uint32_t return_int_from_float(float);


// Flag push/pop/compare and helper functions
void push_flags(r3x_cpu_t*);
void pop_flags(r3x_cpu_t*);
void compare_and_set_flag_signed(r3x_cpu_t*, int, int);
void compare_and_set_flag_unsigned(r3x_cpu_t*, unsigned int, unsigned int);
void set_interrupt(uint8_t interrupt, r3x_cpu_t* CPU);
void handle_cpu_exception(r3x_cpu_t*, unsigned int);
void set_exception_handlers(r3x_cpu_t*, unsigned int, uint32_t);
void r3x_syscall(r3x_cpu_t* CPU);


// Something to print string to buffer
void printfstring(char*, const char * format, ... );


// Let's call it, uh, bitutils? ;)
uint32_t set_bit32(uint32_t, int);
uint32_t unset_bit32(uint32_t, int);
uint32_t toggle_bit32(uint32_t, int);
bool     test_bit32(uint32_t, int);


// CPU Emulation Funciton
int r3x_emulate_instruction(r3x_cpu_t*);

// Keyboard Thread
int keyboard_thread(void* data);

int r3x_cpu_loop(r3x_cpu_t* CPU, r3x_header_t* header)
{
	r3x_dispatch_job(BIOS_START, 1, CPU->RootDomain, true);
	CPU->RootDomain->CurrentJobID = 0;
	r3x_load_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
	CPU->Regs[0] = header->r3x_init;
	r3x_save_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
	// Initialise keyboard thread.
	#ifdef REX_GRAPHICS
	SDL_Thread *kthread = NULL;
	kthread = SDL_CreateThread(keyboard_thread, NULL );
	#endif
	code = 0;
	double milliseconds = 0;
	if(CPU->use_frequency == true){
		milliseconds = (1 / CPU->CPUFrequency);
		printf("CPU Frequency %f\n", milliseconds);
	}
	while (CPU->InstructionPointer < CPU->MemorySize && code != CPU_EXIT_SIGNAL && code != CPU_INVALID_OPCODE_SIGNAL){
			if(milliseconds != 0) {
				cpu_sleep(milliseconds, SLEEP_MILLISECONDS);
			}
			if(exitcalled == true) { 
				break;
			}
			int i = r3x_load_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
			if(i != -1) {
				code = r3x_emulate_instruction(CPU);
				r3x_save_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
			}
			if(CPU->RootDomain->CurrentJobID >= CPU->RootDomain->TotalNumberOfJobs) { 
				CPU->RootDomain->CurrentJobID = -1;
			}
			CPU->RootDomain->CurrentJobID++;
	}
	// Kill it, We're done.
	#ifdef REX_GRAPHICS
	SDL_KillThread(kthread);
	#endif
	return 0;
}
int r3x_emulate_instruction(r3x_cpu_t* CPU)
{
	CPU->CurrentInstruction = CPU->Memory[CPU->InstructionPointer];
	switch (CPU->Memory[CPU->InstructionPointer])
	{
		// Sleep: Delays the CPU by a cycle. Also used by CPU to skip empty memory, as the opcode is 0x0
		case R3X_SLEEP:
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Push: Push a 32-bit value to stack.
		case R3X_PUSH:
			Stack.Push(CPU->Stack, return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		// Pop: Pop a 32-bit value from stack
		case R3X_POP:
			Stack.Pop(CPU->Stack);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Pop number of integers from stack
		case R3X_POPN:
			for(unsigned int i = 0; i < return_32bit_int_from_ip(CPU); i++){
				Stack.Pop(CPU->Stack);
			}
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		// Push the FLAGS register on stack
		case R3X_PUSHF:
			push_flags(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_POPF:
			pop_flags(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Duplicate the value on the top of stack
		case R3X_DUP:
			Stack.Push(CPU->Stack, get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Add: Add 2 values on stack.
		case R3X_ADD:
			Stack.Push(CPU->Stack, get_item_from_stack_top(1) + get_item_from_stack_top(2));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Subtract the second last value on stack by last.
		case R3X_SUB:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) - Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Multiply 2 values on stack.
		case R3X_MUL:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) * Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Divide the second last value by last.
		case R3X_DIV:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) / Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Float equivalents of the above
		case R3X_FADD:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) + return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FSUB:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) - return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FMUL:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) * return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FDIV:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) / return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Compare 2 values on stack and set flags..
		case R3X_CMP:
			compare_and_set_flag_unsigned(CPU, get_item_from_stack_top(2), get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_CMPS:
			compare_and_set_flag_signed(CPU, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2), Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Jump, Jump If equal, Jump if lesser, jump if greater, jump if zero..
		case R3X_JMP:
			CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			break;
		case R3X_JE:
			if (CPU->EqualFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			break;
		case R3X_JL:
			if (CPU->LesserFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			break;
		case R3X_JG:
			if (CPU->GreaterFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			break;
		case R3X_JZ:
			if (CPU->ZeroFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			break;
		// Call VM specific function
		case R3X_SYSCALL:
			r3x_syscall(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		// Load a value from stack offset.
		case R3X_LOADS:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - return_32bit_int_from_ip(CPU)));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		// Load a value and push it to stack, whose address was pushed to stack (32-bit)
		case R3X_LOAD:
			if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;			
			}
			Stack.Push(CPU->Stack, *(uint32_t*)(CPU->Memory + get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Store a value which was pushed to stack to an address pushed second last.
		case R3X_STORE:
			if((unsigned int)get_item_from_stack_top(2) > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;		
			}
			*((uint32_t*)&(CPU->Memory[get_item_from_stack_top(2)])) = get_item_from_stack_top(1);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Memory access for registers...
		case R3X_LODSB:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;		
			}
			CPU->Regs[1] = CPU->Memory[CPU->Regs[0]];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LODSW:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;			
			}
			CPU->Regs[1] = *((uint16_t*)(&CPU->Memory[CPU->Regs[0]]));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LODSD:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;		
			}
			CPU->Regs[1] = *((uint32_t*)(&CPU->Memory[CPU->Regs[0]]));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STOSB:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;			
			}
			CPU->Memory[CPU->Regs[0]] = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STOSW:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			*((uint16_t*)(&CPU->Memory[CPU->Regs[0]])) = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STOSD:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			*((uint32_t*)(&CPU->Memory[CPU->Regs[0]])) = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_CMPSB:
			if(CPU->Regs[3] > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, CPU->Memory[CPU->Regs[3]], (uint8_t)CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_CMPSW:
			if(CPU->Regs[3] > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, *(uint16_t*)(&CPU->Memory[CPU->Regs[3]]), CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_CMPSD:
			if(CPU->Regs[3] > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, *(uint32_t*)(&CPU->Memory[CPU->Regs[3]]), CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Relocatable versions of those..
		case R3X_STOSB_RELOC:
			if(CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			CPU->Memory[CPU->Regs[0] + return_32bit_int_from_ip(CPU)] = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_STOSW_RELOC:
			if(CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			*((uint16_t*)(&CPU->Memory[CPU->Regs[0] + return_32bit_int_from_ip(CPU)])) = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_STOSD_RELOC:
			if(CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			*((uint32_t*)(&CPU->Memory[CPU->Regs[0] + return_32bit_int_from_ip(CPU)])) = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_LODSD_RELOC:
			if((unsigned int)CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;			
			}
			CPU->Regs[1] = *((uint32_t*)(&CPU->Memory[CPU->Regs[0]+return_32bit_int_from_ip(CPU)]));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_LODSW_RELOC:
			if((unsigned int)CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;		
			}
			CPU->Regs[1] = *((uint16_t*)(&CPU->Memory[CPU->Regs[0]+return_32bit_int_from_ip(CPU)]));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_LODSB_RELOC:
			if((unsigned int)CPU->Regs[0] + return_32bit_int_from_ip(CPU) > CPU->MemorySize){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			CPU->Regs[1] = CPU->Memory[CPU->Regs[0]+return_32bit_int_from_ip(CPU)];
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_CMPSB_RELOC:
			if((unsigned int)CPU->Regs[3]+return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, CPU->Memory[CPU->Regs[3]], (uint8_t)CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_CMPSW_RELOC:
			if((unsigned int)CPU->Regs[3]+return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, CPU->Memory[CPU->Regs[3]], (uint16_t)CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_CMPSD_RELOC:
			if((unsigned int)CPU->Regs[3]+return_32bit_int_from_ip(CPU) > CPU->MemorySize) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;
			}
			compare_and_set_flag_unsigned(CPU, *(uint32_t*)(&CPU->Memory[CPU->Regs[3]+return_32bit_int_from_ip(CPU)]), CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		// Bitwise operations
		case R3X_AND:
			Stack.Push(CPU->Stack, get_item_from_stack_top(2) & get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_OR:
			Stack.Push(CPU->Stack, get_item_from_stack_top(2) | get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_XOR:
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) ^ (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_NOT: 
			Stack.Push(CPU->Stack, ~get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_NEG:
			Stack.Push(CPU->Stack, !get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Always set your right foot first soldier! #weirdsuperstitions
		case R3X_SHR:
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) >> (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_SHL:
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) << (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_TERN:
			if(get_item_from_stack_top(3)!=R3X_FALSE){
				// If it's true pop out the first var
				Stack.Pop(CPU->Stack);
			} else {
				Stack.SetItem(CPU->Stack, CPU->Stack->top_of_stack-2, get_item_from_stack_top(1));
				Stack.Pop(CPU->Stack);
			}
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ROR:
			Stack.Push(CPU->Stack, ROR_C(get_item_from_stack_top(2), get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ROL:
			Stack.Push(CPU->Stack, ROL_C(get_item_from_stack_top(2), get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Native library support (calling native procedures from .so files)
		case R3X_LOADLIB:
			if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);		
				break;	
			}
			#ifdef REX_DYNAMIC
				load_native_library((char*)(CPU->Memory + get_item_from_stack_top(1)), CPU);
			#else
				printf("Not compiled with native dynamic library support. Attempt to call LOADLIB, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LIBEXEC:
			if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);	
				break;		
			}
			if((unsigned int)get_item_from_stack_top(2) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);	
				break;		
			}
			#ifdef REX_DYNAMIC
			Stack.Push(CPU->Stack, native_call((char*)(CPU->Memory + get_item_from_stack_top(1)), returnhandle((char*)(CPU->Memory + get_item_from_stack_top(2)))));
			#else
			printf("Not compiled with native dynamic library support. Attempt to call LIBEXEC, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
		// Call, ret and call stack operations
		case R3X_CALL:
			Stack.Push(CPU->CallStack, CPU->InstructionPointer + CPU_INCREMENT_WITH_32_OP);
			CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			break;
		case R3X_RET:
			CPU->InstructionPointer = Stack.Pop(CPU->CallStack);
			break;
		case R3X_PUSHA:
			Stack.Push(CPU->CallStack, return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_POPA:
			Stack.Pop(CPU->CallStack);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// fast memory copy
		case R3X_MEMCPY:
			if((unsigned int)get_item_from_stack_top(3)+(unsigned int)get_item_from_stack_top(1) > CPU->MemorySize || (unsigned int)get_item_from_stack_top(2) + (unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					break;			
			}
			memcpy(CPU->Memory + get_item_from_stack_top(3), CPU->Memory + get_item_from_stack_top(2), get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Register operations
		case R3X_LOADR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {	
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer])] = return_32bit_int_from_ip(CPU);	
			}
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_PUSHR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				Stack.Push(CPU->Stack, CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])]);
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		case R3X_POPR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				int a = Stack.Pop(CPU->Stack);
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] = a;
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;	
		case R3X_INCR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] += 1;
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		case R3X_DECR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] -= 1;
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		// Program interruption
		case R3X_INT:
			if(CPU->ISR_handlers[CPU->Memory[CPU->InstructionPointer+1]] != 0) {
				Stack.Push(CPU->CallStack, CPU->InstructionPointer + CPU_INCREMENT_DOUBLE);
				CPU->InstructionPointer = CPU->ISR_handlers[CPU->Memory[CPU->InstructionPointer+1]];
			} else {	
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			break;
		case R3X_LOADI:
			CPU->InstructionPointer++;
			set_interrupt(CPU->Memory[CPU->InstructionPointer], CPU);
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_PUSHAR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				Stack.Push(CPU->CallStack, CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])]);
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		case R3X_POPAR:
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				int a = Stack.Pop(CPU->CallStack);
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] = a;
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		// Support for REX object files (*.ro)
		case R3X_CALLDYNAMIC:	
			if(dynamic_call(get_item_from_stack_top(2), get_item_from_stack_top(1))==0)	{
				printf("Invalid dynamic call, causing CPU_EXCEPTION_INVALIDACCESS.\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				break;			
			}
			Stack.Push(CPU->CallStack, CPU->InstructionPointer+1);
			CPU->InstructionPointer = dynamic_call(get_item_from_stack_top(2), get_item_from_stack_top(1));
			break;
		// Math Instructions
		case R3X_FSIN:
			Stack.Push(CPU->Stack, return_int_from_float(sin(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FCOS:
			Stack.Push(CPU->Stack, return_int_from_float(cos(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FTAN:
			Stack.Push(CPU->Stack, return_int_from_float(tan(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ASIN:
			Stack.Push(CPU->Stack, return_int_from_float(asin(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ACOS:
			Stack.Push(CPU->Stack, return_int_from_float(acos(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ATAN:
			Stack.Push(CPU->Stack, return_int_from_float(atan(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FPOW:
			Stack.Push(CPU->Stack, return_int_from_float(pow(return_float(get_item_from_stack_top(2)), return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Convert to angle 
		case R3X_ACONV:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(get_item_from_stack_top(1))*(180.0/M_PI)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Convert to radian
		case R3X_RCONV:
			Stack.Push(CPU->Stack, return_int_from_float(return_float(get_item_from_stack_top(1))*(M_PI/180.0)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_MOD:
			Stack.Push(CPU->Stack, get_item_from_stack_top(2) % get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_FMOD:
			Stack.Push(CPU->Stack, return_int_from_float(fmod(return_float(get_item_from_stack_top(2)), return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Epic-ass, first class, sexy error handling
		case R3X_CATCH:
			set_exception_handlers(CPU, get_item_from_stack_top(1), get_item_from_stack_top(2));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_THROW:
			handle_cpu_exception(CPU, get_item_from_stack_top(1));
			break;
		case R3X_HANDLE:
			CPU->ExceptionFlag = false;
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		// Exit application
		case R3X_EXIT:
			if(CPU->RootDomain->Jobs[CPU->RootDomain->CurrentJobID]->ismain == true) {		
				r3x_exit_job(CPU->RootDomain, CPU->RootDomain->CurrentJobID);
				return CPU_EXIT_SIGNAL; // Main... exitted close everything and return...
			}
			r3x_exit_job(CPU->RootDomain, CPU->RootDomain->CurrentJobID);
			return 0;
		default:
			printf("Unknown Opcode: %x, IP: %u\n", (unsigned int)CPU->Memory[CPU->InstructionPointer], CPU->InstructionPointer);
			handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			break;
	}
	return 0;
}
uint32_t return_32bit_int(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	__32bit_typecast temp_typecast32;
	temp_typecast32.__num32 = 0;
	temp_typecast32.__num8.a = a;
	temp_typecast32.__num8.b = b;
	temp_typecast32.__num8.c = c;
	temp_typecast32.__num8.d = d;
	return temp_typecast32.__num32;
}
float return_float(uint32_t num32)
{	
	__float_typecast temp_typecastFL;;
	temp_typecastFL.float32 = 0.0f;
	temp_typecastFL.__num32 = num32;
	return temp_typecastFL.float32;
}
uint32_t return_int_from_float(float fl32)
{
	__float_typecast temp_typecastFL;
	temp_typecastFL.float32 = 0.0f;
	temp_typecastFL.__num32 = 0;
	temp_typecastFL.float32 = fl32;
	return temp_typecastFL.__num32;
}
uint32_t return_32bit_int_from_ip(r3x_cpu_t* CPU)
{
	uint8_t a = CPU->Memory[CPU->InstructionPointer+1];
	uint8_t b = CPU->Memory[CPU->InstructionPointer+2];
	uint8_t c = CPU->Memory[CPU->InstructionPointer+3];
	uint8_t d = CPU->Memory[CPU->InstructionPointer+4];
	return return_32bit_int(a, b, c, d);
}
void compare_and_set_flag_signed(r3x_cpu_t* CPU, signed int op1, signed int op2)
{
	// False out all flags
	CPU->EqualFlag = false;
	CPU->ZeroFlag = false;
	CPU->GreaterFlag = false;
	CPU->LesserFlag = false;
	// Compare values and set CPU flags accordingly
	if (op1 == op2){
		CPU->EqualFlag = true;
	}
	if (op1 == 0){
		CPU->ZeroFlag = true;
	}
	if (op1 > op2){
		CPU->GreaterFlag = true;
	}
	if (op1 < op2){
		CPU->LesserFlag = true;
	}
}
void compare_and_set_flag_unsigned(r3x_cpu_t* CPU, unsigned int op1, unsigned int op2)
{
	// False out all flags
	CPU->EqualFlag = false;
	CPU->ZeroFlag = false;
	CPU->GreaterFlag = false;
	CPU->LesserFlag = false;
	// Compare values and set CPU flags accordingly
	if (op1 == op2){
		CPU->EqualFlag = true;
	}
	if (op1 == 0){
		CPU->ZeroFlag = true;
	}
	if (op1 > op2){
		CPU->GreaterFlag = true;
	}
	if (op1 < op2){
		CPU->LesserFlag = true;
	}
}
void set_interrupt(uint8_t interrupt, r3x_cpu_t* CPU) {
	if(return_32bit_int_from_ip(CPU) > CPU->MemorySize) {	
		raise(SIGSEGV);	
	}
	CPU->ISR_handlers[interrupt] = return_32bit_int_from_ip(CPU);
}
void printfstring(char* string, const char * format, ... )
{
  va_list args;
  va_start (args, format);
  vsprintf (string,format, args);
  va_end (args);
}
int keyboard_thread(void* data) { 
	(void)data;
	#ifdef REX_GRAPHICS
	while(true) {
		if(SDL_WaitEvent(&key_event))
		{
			if(key_event.type == SDL_QUIT)
			{
				exitcalled = true;
			}
			//If a key was pressed
			else if(key_event.type == SDL_KEYDOWN)
			{
				
				if(is_read == true)  {
					//SDL_EnableUNICODE( SDL_ENABLE );
					keycode = (char)key_event.key.keysym.unicode;
					is_read = false;
				}
			}
		}	
		if(is_read == true)  { 
				keycode = 0;
		}
	}
	#endif
	return 0;
}
void r3x_syscall(r3x_cpu_t* CPU) { 
		if (CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTS){
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				#ifdef REX_GRAPHICS
				vm_puts(CPU->Graphics->font, (char*)(CPU->Memory + get_item_from_stack_top(1)), CPU->Graphics);	
				#else
				printf("%s", (char*)(CPU->Memory + get_item_from_stack_top(1)));
				#endif
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTI){
				 char buffer[33];
				 memset(buffer, 0, 33);
				 printfstring(buffer, "%u ", (unsigned int)get_item_from_stack_top(1));
				 #ifdef REX_GRAPHICS
				 vm_puts(CPU->Graphics->font, buffer, CPU->Graphics);
				 #else
				 printf("%s", buffer);
				 #endif

			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTF){
				char buffer[33];
				memset(buffer, 0, 33);
				printfstring(buffer, "%f ", (float)return_float(get_item_from_stack_top(1)));
				 #ifdef REX_GRAPHICS
				 vm_puts(CPU->Graphics->font, buffer, CPU->Graphics);
				 #else
				 printf("%s", buffer);
				 #endif
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_GETC){
				#ifdef REX_GRAPHICS
				// Ensure the there !IS! a keyboard interrupt, our thread will set the is_read value to false if there is.
				if(is_read == false) {
					is_read = true; 
					Stack.Push(CPU->Stack, keycode);
				}
				// To prevent race condition, push 0 if is_read == false;
				else {		
					
					Stack.Push(CPU->Stack, 0);				
				}
				#else 
					//int c = getchar();	
					Stack.Push(CPU->Stack, getchar());
				#endif
				
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_GLUPDATE){
				#ifdef REX_GRAPHICS
				gl_text_update(CPU->Graphics);
				#endif		
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTCH){
				#ifdef REX_GRAPHICS
				vm_putc((char)get_item_from_stack_top(1), CPU->Graphics);
				gl_text_update(CPU->Graphics);			
				#else
				printf("%c", (char)get_item_from_stack_top(1));
				#endif
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_ATOI) {
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, atoi((char*)&CPU->Memory[get_item_from_stack_top(1)]));			
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_ALLOC) { 
				if((unsigned int)get_item_from_stack_top(1) > 4096) { 
					printf("Attempt to allocate memory more than 4096 bytes at once\n");
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}	
				Stack.Push(CPU->Stack, CPU->MemorySize);
				CPU->MemorySize += get_item_from_stack_top(1);
				CPU->Memory = nt_realloc(CPU->Memory, CPU->MemorySize);
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_DISPATCH) {
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize){
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				r3x_dispatch_job(get_item_from_stack_top(1), 1, CPU->RootDomain, true);
							
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_LOADDYNAMIC) { 
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, load_dynamic_library((char*)(CPU->Memory + get_item_from_stack_top(1)), CPU));			
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_OPENSTREAM) { 
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, stream_open((char*)(&CPU->Memory[get_item_from_stack_top(1)])));
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_CLOSESTREAM) { 
				stream_close((unsigned int)get_item_from_stack_top(1));
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_SEEKSTREAM) {
				stream_seek((unsigned int)get_item_from_stack_top(3), (long int)get_item_from_stack_top(2), get_item_from_stack_top(1));
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_READSTREAM) {
				if((unsigned int)(get_item_from_stack_top(2)+get_item_from_stack_top(1)) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, stream_read((void*)(&CPU->Memory[get_item_from_stack_top(2)]), get_item_from_stack_top(3), get_item_from_stack_top(1))); 
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_WRITESTREAM) {
				if((unsigned int)(get_item_from_stack_top(2)+get_item_from_stack_top(1)) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, stream_write((void*)(&CPU->Memory[get_item_from_stack_top(2)]), get_item_from_stack_top(3), get_item_from_stack_top(1))); 
			}
			else {
				printf("Invalid Argument passed to R3X_SYSCALL\n");
			}
}
uint32_t set_bit32(uint32_t num, int bit){
	num |= 1 << bit;
	return num;
}
uint32_t unset_bit32(uint32_t num, int bit){
	num &= ~(1 << bit);
	return num;
}
uint32_t toggle_bit32(uint32_t num, int bit){
	num ^= 1 << bit;
	return num;
}
bool test_bit32(uint32_t num, int bit){
	num = !!(num & (1 << bit));
	if(num == 0){ return false; }
	return true;
}
void push_flags(r3x_cpu_t* CPU){
	CPU->FLAGS = 0x00000000;
	if(CPU->EqualFlag == true){
		CPU->FLAGS = set_bit32(CPU->FLAGS, EFLAG_BIT);
	}
	if(CPU->GreaterFlag == true){
		CPU->FLAGS = set_bit32(CPU->FLAGS, GFLAG_BIT);
	}
	if(CPU->LesserFlag == true){
		CPU->FLAGS = set_bit32(CPU->FLAGS, LFLAG_BIT);
	}
	if(CPU->ZeroFlag == true){
		CPU->FLAGS = set_bit32(CPU->FLAGS, ZFLAG_BIT);
	}
	if(CPU->ExceptionFlag == true){
		CPU->FLAGS = set_bit32(CPU->FLAGS, EXFLAG_BIT);
	}
	Stack.Push(CPU->Stack, CPU->FLAGS);
}
void pop_flags(r3x_cpu_t* CPU){
	uint32_t flags = (uint32_t)get_item_from_stack_top(1);
	CPU->EqualFlag = false; CPU->GreaterFlag = false; CPU->LesserFlag = false; CPU->ZeroFlag = false;
	CPU->ExceptionFlag = false;
	if(test_bit32(flags, EFLAG_BIT)==true){
		CPU->EqualFlag = true;
	} 
	if(test_bit32(flags, GFLAG_BIT)==true){
		CPU->GreaterFlag = true;
	}
	if(test_bit32(flags, LFLAG_BIT)==true){
		CPU->LesserFlag = true;
	}
	if(test_bit32(flags, ZFLAG_BIT)==true){
		CPU->ZeroFlag = true;
	}
	if(test_bit32(flags, EXFLAG_BIT)==true){
		CPU->ExceptionFlag = true;
	}
	Stack.Pop(CPU->Stack);
}
void set_exception_handlers(r3x_cpu_t* CPU, unsigned int ExceptionID, uint32_t ExceptionHandler){
	if(ExceptionID > TOTAL_EXCEPTIONS){
			printf("Process %u tried to set an invalid exception handler\n", CPU->RootDomain->CurrentJobID);
			return;
	} else {
			printf("Process is setting exception handler %u to IP %u\n", ExceptionID, ExceptionHandler);
			CPU->ExceptionHandlers[ExceptionID] = ExceptionHandler;
	}
}
void handle_cpu_exception(r3x_cpu_t* CPU, unsigned int ExceptionID){
	if(CPU->ExceptionFlag==true){
		printf("Bad Exception handler, IP: %u. ABORTING...\n", CPU->InstructionPointer);
		raise(SIGSEGV);
	}
 	if(CPU->ExceptionHandlers[ExceptionID]==0){
		printf("Unhandled Exception!\nException Type: ");
		switch(ExceptionID){
			case CPU_EXCEPTION_INVALIDACCESS:
				printf("Invalid memory access by program\n");
				break;
			case CPU_EXCEPTION_INVALIDOPCODE:
				printf("Attempt to Execute an invalid instruction\n");
				break;
			case CPU_EXCEPTION_EXCEPTION:
				printf("Unhandled exception thrown by program itself\n");
				break;
			default:
				printf("Unknown Exception!\n");
				break;
		}
		raise(SIGSEGV);
	} else {
		CPU->InstructionPointer = CPU->ExceptionHandlers[ExceptionID];
		CPU->ExceptionFlag = true;
		return;
	}
}