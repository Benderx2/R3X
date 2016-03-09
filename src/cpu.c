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
#ifdef REX_OPTIMIZE
#include <pthread.h>
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
const int ARGUMENT_LOCATION = 4096;
// Bools and global stuff
bool exitcalled = false;
bool incycle = false;
bool is_read = true;
char keycode = 0;

#ifdef REX_GRAPHICS
SDL_Event key_event = {SDL_USEREVENT};
#endif

// Helpful macros
#define get_item_from_stack_top(x) (uint64_t)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-x)
#define cpu_sleep(time, unit) usleep(time*unit)


// Rotate ints
#define  ROL_C(num, bits) ((uint32_t)num << (uint32_t)bits) | ((uint32_t)num >> (32 - (uint32_t)bits))
#define  ROR_C(num, bits) ((uint32_t)num >> (uint32_t)bits) | ((uint32_t)num << (32 - (uint32_t)bits))


// <type>-to-uint32 helpers
static inline uint32_t return_32bit_int(uint8_t, uint8_t, uint8_t, uint8_t);
static inline uint32_t return_32bit_int_from_ip(r3x_cpu_t*);
static inline float return_float(uint32_t);
static inline uint32_t return_int_from_float(float);


// Flag push/pop/compare and helper functions
static inline void push_flags(r3x_cpu_t*);
static inline void pop_flags(r3x_cpu_t*);
static inline void compare_and_set_flag_signed(r3x_cpu_t*, int, int);
static inline void compare_and_set_flag_unsigned(r3x_cpu_t*, unsigned int, unsigned int);
static inline void set_interrupt(uint8_t interrupt, r3x_cpu_t* CPU);
void set_exception_handlers(r3x_cpu_t*, unsigned int, uint32_t);
static inline void r3x_syscall(r3x_cpu_t* CPU);
static inline int64_t r3x_ars(int64_t x, int64_t n);

// Something to print string to buffer
static inline void printfstring(char*, const char * format, ... );


// Let's call it, uh, bitutils? ;)
static inline uint32_t set_bit32(uint32_t, int);
static inline uint32_t unset_bit32(uint32_t, int);
static inline uint32_t toggle_bit32(uint32_t, int);
static inline bool     test_bit32(uint32_t, int);


// CPU Emulation Funciton
static inline void r3x_emulate_instruction(r3x_cpu_t*);
void* CPUDispatchThread(void*);

// Keyboard Thread
int keyboard_thread(void* data);

/*!
 * r3x_cpu_loop - Calls r3x_emulate_instruction repeateadly until it reaches a non-exec region
 * or an exit instruction. 
*/
int r3x_cpu_loop(register r3x_cpu_t* CPU, r3x_header_t* header, char* Arguments)
{
	r3x_dispatch_job(BIOS_START, 1, CPU->RootDomain, true);
	CPU->RootDomain->CurrentJobID = 0;
	r3x_load_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
	CPU->Regs[0] = header->r3x_init;
	r3x_save_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID);
	//! if less than 500, copy the length
	if(strlen(Arguments) < 500) {
		memcpy((char*)&CPU->Memory[ARGUMENT_LOCATION], Arguments, strlen(Arguments));
	} else {
		//! else copy only 500 bytes.
		memcpy((char*)&CPU->Memory[ARGUMENT_LOCATION], Arguments, 500);
	}
	// Initialise keyboard thread.
	#ifdef REX_GRAPHICS
	SDL_Thread *kthread = NULL;
	kthread = SDL_CreateThread(keyboard_thread, NULL );
	#endif
	#ifdef REX_OPTIMIZE
	pthread_t CPUDispatchThread_handle;
	if(pthread_create(&CPUDispatchThread_handle, NULL, &CPUDispatchThread, (void*)CPU) == -1){
		printf("Unable to create tertiary CPU thread!\n");
		exit(EXIT_FAILURE);
	}
	#endif
	double milliseconds = 0;
	if(CPU->use_frequency == true){
		milliseconds = (1 / CPU->CPUFrequency);
		printf("CPU Frequency %f\n", milliseconds);
	}
	struct timespec tp;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
	CPU->CPUClock = tp.tv_sec * CLOCKS_PER_SEC;
	while (exitcalled == false){
			#ifdef R_DEBUG
			if(milliseconds != 0) {
				cpu_sleep(milliseconds, SLEEP_MILLISECONDS);
			}
			#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->InstructionPointer) != RX_EXEC){
				printf("Error: (INSTRUCTION FETCH FAULT) Attempt to execute code in a non-exec region, IP: 0x%X\n", CPU->InstructionPointer);
				raise(SIGSEGV);
				break;
			}
			if(r3x_load_job_state(CPU, CPU->RootDomain, CPU->RootDomain->CurrentJobID) != -1) {
				/**
				 * Is broken. Need to find another way of speeding up execution :P
				 * 
				**/
				r3x_emulate_instruction(CPU);
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
	#ifdef REX_OPTIMIZE
	/*
	 * Nice, no shit needed.
	 * */
	#ifdef __unix__
	pthread_kill(CPUDispatchThread_handle, SIGKILL);
	#else
	#warning "lolwut."
	/*
	 * Fuck off, non-POZIX platformz
	 */
	pthread_kill(CPUDispatchThread_handle, SIGTERM);
	#endif
	#endif
	return 0;
}

/**
 * r3x_emulate_instruction - Emulate a single instruction, and update the CPU variable.
**/
static inline void r3x_emulate_instruction(register r3x_cpu_t* CPU)
{
	if(exitcalled == true || CPU->RootDomain->Jobs[CPU->RootDomain->CurrentJobID] == NULL) {
	    return;
	}
	#ifndef REX_OPTIMIZE
	switch (CPU->Memory[CPU->InstructionPointer])
	{
	#else
	#include <r3x_opcode_table.h>
	goto *DispatchTable[CPU->Memory[CPU->InstructionPointer]];
	#endif
		// Sleep: Delays the CPU by a cycle. Also used by CPU to skip empty memory, as the opcode is 0x0
		#ifndef REX_OPTIMIZE
		case R3X_SLEEP:
		#else 
		INTERP_SLEEP:
		#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
		#ifndef REX_OPTIMIZE
			break;
		#else 
			return;
		#endif
		// Push: Push a 32-bit value to stack.
		#ifndef REX_OPTIMIZE
		case R3X_PUSH:
		#else
		INTERP_PUSH:
		#endif
			Stack.Push(CPU->Stack, return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
		#ifndef REX_OPTIMIZE
			break;
		#else 
			return;
		#endif
		// Pop: Pop a 32-bit value from stack
		#ifndef REX_OPTIMIZE
		case R3X_POP:
		#else
		INTERP_POP:
		#endif
			Stack.Pop(CPU->Stack);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
		#ifndef REX_OPTIMIZE
			break;
		#else 
			return;
		#endif
		// Pop number of integers from stack
		#ifndef REX_OPTIMIZE
		case R3X_POPN:
		#else
		INTERP_POPN:
		#endif
			for(unsigned int i = 0; i < return_32bit_int_from_ip(CPU); i++){
				Stack.Pop(CPU->Stack);
			}
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Push the FLAGS register on stack
		#ifndef REX_OPTIMIZE
		case R3X_PUSHF:
		#else
		INTERP_PUSHF:
		#endif
			push_flags(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Pop flags from stack
		#ifndef REX_OPTIMIZE
		case R3X_POPF:
		#else 
		INTERP_POPF:
		#endif
			pop_flags(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Duplicate the value on the top of stack
		#ifndef REX_OPTIMIZE
		case R3X_DUP:
		#else
		INTERP_DUP:
		#endif
			Stack.Push(CPU->Stack, get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Add: Add 2 values on stack.
		#ifndef REX_OPTIMIZE
		case R3X_ADD:
		#else
		INTERP_ADD:
		#endif
			Stack.Push(CPU->Stack, get_item_from_stack_top(1) + get_item_from_stack_top(2));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Subtract the second last value on stack by last.
		#ifndef REX_OPTIMIZE
		case R3X_SUB:
		#else
		INTERP_SUB:
		#endif
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) - Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Multiply 2 values on stack.
		#ifndef REX_OPTIMIZE
		case R3X_MUL:
		#else
		INTERP_MUL:
		#endif
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) * Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Divide the second last value by last.
		#ifndef REX_OPTIMIZE
		case R3X_DIV:
		#else
		INTERP_DIV:
		#endif
			if(get_item_from_stack_top(1) == 0) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_ARITHMETIC);
			} else {
				Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) / Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Float equivalents of the above
		#ifndef REX_OPTIMIZE
		case R3X_FADD:
		#else
		INTERP_FADD:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) + return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FSUB:
		#else
		INTERP_FSUB:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) - return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FMUL:
		#else
		INTERP_FMUL:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) * return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FDIV:
		#else
		INTERP_FDIV:
		#endif
			if(return_float(get_item_from_stack_top(1)) == 0.0f) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_ARITHMETIC);
			} else {
				Stack.Push(CPU->Stack, return_int_from_float(return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2)) / return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1))));
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// sete/setne/setl/setg
		#ifndef REX_OPTIMIZE
		case R3X_SETE:
		#else
		INTERP_SETE:
		#endif
			if(CPU->EqualFlag==true) {
				if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
					CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]] = 1;
					CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
				} else {
					printf("Invalid Register Index\n");
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_SETNE:
		#else
		INTERP_SETNE:
		#endif
			if(CPU->EqualFlag!=true) {
				if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
					CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]] = 1;
					CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
				} else {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_SETL:
		#else
		INTERP_SETL:
		#endif
			if(CPU->LesserFlag==true) {
				if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
					CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]] = 1;
					CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
				} else {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_SETG:
		#else
		INTERP_SETG:
		#endif
			if(CPU->GreaterFlag==true) {
				if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
					CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]] = 1;
					CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
				} else {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Compare 2 values on stack and set flags..
		#ifndef REX_OPTIMIZE
		case R3X_CMP:
		#else
		INTERP_CMP:
		#endif
			compare_and_set_flag_unsigned(CPU, get_item_from_stack_top(2), get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_CMPS:
		#else
		INTERP_CMPS:
		#endif
			compare_and_set_flag_signed(CPU, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2), Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Call VM specific function
		#ifndef REX_OPTIMIZE
		case R3X_SYSCALL:
		#else
		INTERP_SYSCALL:
		#endif
			r3x_syscall(CPU);
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Load a value from stack offset.
		#ifndef REX_OPTIMIZE
		case R3X_LOADS:
		#else
		INTERP_LOADS:
		#endif
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - return_32bit_int_from_ip(CPU)));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LOADSR:
		#else
		INTERP_LOADSR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS){
				Stack.Push(CPU->Stack, get_item_from_stack_top(CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]]));
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Store a value to stack offset
		#ifndef REX_OPTIMIZE
		case R3X_STORES:
		#else
		INTERP_STORES:
		#endif
			if(Stack.SetItem(CPU->Stack, CPU->Stack->top_of_stack-return_32bit_int_from_ip(CPU), get_item_from_stack_top(1))==-1){
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			}
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_STORESR:
		#else
		INTERP_STORESR:
		#endif
			// TODO: Stop using hacks!
			get_item_from_stack_top(1);
			uint32_t val = 0;
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS){
				val = CPU->Regs[CPU->Memory[CPU->InstructionPointer+1]];
				if(Stack.SetItem(CPU->Stack, CPU->Stack->top_of_stack-val, get_item_from_stack_top(1))==-1){
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
				}
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		/**!
		 * Here starts the region where memory access instructions are used!
		 *
		 * It may change due to addition of virtual memory addressesing
		 * **/
		//! Jump, Jump If equal, Jump if lesser, jump if greater, jump if zero, call, ret (That is, all branch instructions)..
		#ifndef REX_OPTIMIZE
		case R3X_JMP:
		#else
		INTERP_JMP:
		#endif
			CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JE:
		#else
		INTERP_JE:
		#endif
			if (CPU->EqualFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JL:
		#else
		INTERP_JL:
		#endif
			if (CPU->LesserFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JG:
		#else
		INTERP_JG:
		#endif
			if (CPU->GreaterFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JZ:
		#else
		INTERP_JZ:
		#endif
			if (CPU->ZeroFlag == true){
				CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			}
			else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Relative jumps
		#ifndef REX_OPTIMIZE
		case R3X_JMPL:
		#else
		INTERP_JMPL:
		#endif
			if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
				CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
			} else if((int32_t)return_32bit_int_from_ip(CPU) < 0) {
				CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JEL:
		#else
		INTERP_JEL:
		#endif
			if(CPU->EqualFlag == true){
				if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
					CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
				} else {
					CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JGL:
		#else
		INTERP_JGL:
		#endif
			if(CPU->GreaterFlag == true){
				if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
					CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
				} else {
					CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JLL:
		#else
		INTERP_JLL:
		#endif
			if(CPU->LesserFlag == true){
				if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
					CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
				} else {
					CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_JZL:
		#else
		INTERP_JZL:
		#endif
			if(CPU->ZeroFlag == true){
				if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
					CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
				} else {
					CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
				}
			} else {
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_PUSHIP:
		#else
		INTERP_PUSHIP:
		#endif
			Stack.Push(CPU->Stack, CPU->InstructionPointer);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Call, ret and call stack operations
		#ifndef REX_OPTIMIZE
		case R3X_CALL:
		#else
		INTERP_CALL:
		#endif
			Stack.Push(CPU->CallStack, CPU->InstructionPointer + CPU_INCREMENT_WITH_32_OP);
			CPU->InstructionPointer = return_32bit_int_from_ip(CPU);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Relative call (CALLL)
		#ifndef REX_OPTIMIZE
		case R3X_CALLL:
		#else
		INTERP_CALLL:
		#endif
			Stack.Push(CPU->CallStack, CPU->InstructionPointer + CPU_INCREMENT_WITH_32_OP);
			if((int32_t)return_32bit_int_from_ip(CPU) > 0) {
				CPU->InstructionPointer +=  return_32bit_int_from_ip(CPU);
			} else {
				CPU->InstructionPointer -= abs((int32_t)return_32bit_int_from_ip(CPU));
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_RET:
		#else
		INTERP_RET:
		#endif
			CPU->InstructionPointer = Stack.Pop(CPU->CallStack);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Load a value and push it to stack, whose address was pushed to stack (32-bit)
		#ifndef REX_OPTIMIZE
		case R3X_LOAD:
		#else
		INTERP_LOAD:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, get_item_from_stack_top(1)) != RX_RONLY  || GetBlockTypefromAddress(CPU->CPUMemoryBlocks, get_item_from_stack_top(1)) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			Stack.Push(CPU->Stack, *(uint32_t*)(CPU->Memory + get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Store a value which was pushed to stack to an address pushed second last.
		#ifndef REX_OPTIMIZE
		case R3X_STORE:
		#else
		INTERP_STORE:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, get_item_from_stack_top(1)) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif	
			}
			*((uint32_t*)&(CPU->Memory[get_item_from_stack_top(2)])) = get_item_from_stack_top(1);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Memory access for registers...
		#ifndef REX_OPTIMIZE
		case R3X_LODSB:
		#else
		INTERP_LODSB:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			CPU->Regs[1] = CPU->Memory[CPU->Regs[0]];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LODSW:
		#else
		INTERP_LODSW:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			CPU->Regs[1] = BIG_ENDIAN_INT16(*((uint16_t*)(&CPU->Memory[CPU->Regs[0]])));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LODSD:
		#else
		INTERP_LODSD:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			CPU->Regs[1] = BIG_ENDIAN_INT(*((uint32_t*)(&CPU->Memory[CPU->Regs[0]])));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_STOSB:
		#else
		INTERP_STOSB:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			CPU->Memory[CPU->Regs[0]] = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_STOSW:
		#else
		INTERP_STOSW:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif	
			}
			*((uint16_t*)(&CPU->Memory[CPU->Regs[0]])) = BIG_ENDIAN_INT16(CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_STOSD:
		#else
		INTERP_STOSD:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[0]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
			}
			*((uint32_t*)(&CPU->Memory[CPU->Regs[0]])) = BIG_ENDIAN_INT(CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_CMPSB:
		#else
		INTERP_CMPSB:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
			}
			compare_and_set_flag_unsigned(CPU, CPU->Memory[CPU->Regs[3]], (uint8_t)CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_CMPSW:
		#else
		INTERP_CMPSW:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif	
			}
			compare_and_set_flag_unsigned(CPU, *(uint16_t*)(&CPU->Memory[CPU->Regs[3]]), CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_CMPSD:
		#else
		INTERP_CMPSD:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RONLY && GetBlockTypefromAddress(CPU->CPUMemoryBlocks, CPU->Regs[3]) != RX_RW){ 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif	
			}
			compare_and_set_flag_unsigned(CPU, *(uint32_t*)(&CPU->Memory[CPU->Regs[3]]), CPU->Regs[1]);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! fast memory copy
		#ifndef REX_OPTIMIZE
		case R3X_MEMCPY:
		#else
		INTERP_MEMCPY:
		#endif
			if((unsigned int)get_item_from_stack_top(3)+(unsigned int)get_item_from_stack_top(1) > CPU->MemorySize || (unsigned int)get_item_from_stack_top(2) + (unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif			
			}
			memcpy(CPU->Memory + get_item_from_stack_top(3), CPU->Memory + get_item_from_stack_top(2), get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Native library support (calling native procedures from .so files)
		#ifndef REX_OPTIMIZE
		case R3X_LOADLIB:
		#else
		INTERP_LOADLIB:
		#endif
			if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);		
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
			}
			#ifdef REX_DYNAMIC
				load_native_library((char*)(CPU->Memory + get_item_from_stack_top(1)), CPU);
			#else
				printf("Not compiled with native dynamic library support. Attempt to call LOADLIB, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LIBEXEC:
		#else
		INTERP_LIBEXEC:
		#endif
			if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);	
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			if((unsigned int)get_item_from_stack_top(2) > CPU->MemorySize) { 
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);	
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif		
			}
			#ifdef REX_DYNAMIC
			Stack.Push(CPU->Stack, native_call((char*)(CPU->Memory + get_item_from_stack_top(1)), returnhandle((char*)(CPU->Memory + get_item_from_stack_top(2)))));
			#else
			printf("Not compiled with native dynamic library support. Attempt to call LIBEXEC, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Program interruption
		#ifndef REX_OPTIMIZE
		case R3X_INT:
		#else
		INTERP_INT:
		#endif
			if(CPU->ISR_handlers[CPU->Memory[CPU->InstructionPointer+1]] != 0) {
				Stack.Push(CPU->CallStack, CPU->InstructionPointer + CPU_INCREMENT_DOUBLE);
				CPU->InstructionPointer = CPU->ISR_handlers[CPU->Memory[CPU->InstructionPointer+1]];
			} else {	
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		//! Support for REX object files (*.ro)
		#ifndef REX_OPTIMIZE
		case R3X_CALLDYNAMIC:
		#else
		INTERP_CALLDYNAMIC:
		#endif
			if(GetBlockTypefromAddress(CPU->CPUMemoryBlocks, get_item_from_stack_top(1)) != RX_RW || GetBlockTypefromAddress(CPU->CPUMemoryBlocks, get_item_from_stack_top(1)) != RX_RW) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			}
			uint32_t d_addr = return_dynamic_load_addr(get_item_from_stack_top(2));
			if(d_addr==0) {
				printf("Invalid dynamic call, Unknown handle!\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
			}
			CPU->Regs[20] = d_addr;
			if(dynamic_call(CPU, get_item_from_stack_top(2), (char*)(CPU->Memory + get_item_from_stack_top(1)))==0)	{
				printf("Invalid dynamic call, causing CPU_EXCEPTION_INVALIDACCESS.\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif	
			}
			Stack.Push(CPU->CallStack, CPU->InstructionPointer+1);
			CPU->InstructionPointer = dynamic_call(CPU, get_item_from_stack_top(2), (char*)(CPU->Memory + get_item_from_stack_top(1)));
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		/**!
		 * This is the end of memory acessing instructions section
		 * **/
		// Bitwise operations
		#ifndef REX_OPTIMIZE
		case R3X_AND:
		#else
		INTERP_AND:
		#endif
			Stack.Push(CPU->Stack, get_item_from_stack_top(2) & get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_OR:
		#else
		INTERP_OR:
		#endif
			Stack.Push(CPU->Stack, get_item_from_stack_top(2) | get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_XOR:
		#else
		INTERP_XOR:
		#endif
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) ^ (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_NOT: 
		#else
		INTERP_NOT:
		#endif
			Stack.Push(CPU->Stack, ~get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_NEG:
		#else
		INTERP_NEG:
		#endif
			Stack.Push(CPU->Stack, !get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Always set your right foot first soldier! #weirdsuperstitions
		#ifndef REX_OPTIMIZE
		case R3X_SHR:
		#else
		INTERP_SHR:
		#endif
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) >> (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_SHL:
		#else
		INTERP_SHL:
		#endif
			Stack.Push(CPU->Stack, (unsigned int)get_item_from_stack_top(2) << (unsigned int)get_item_from_stack_top(1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_TERN:
		#else
		INTERP_TERN:
		#endif
			if(get_item_from_stack_top(3)!=R3X_FALSE){
				// If it's true pop out the first var
				Stack.Pop(CPU->Stack);
			} else {
				Stack.SetItem(CPU->Stack, CPU->Stack->top_of_stack-2, get_item_from_stack_top(1));
				Stack.Pop(CPU->Stack);
			}
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ROR:
		#else
		INTERP_ROR:
		#endif
			Stack.Push(CPU->Stack, ROR_C(get_item_from_stack_top(2), get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ROL:
		#else
		INTERP_ROL:
		#endif
			Stack.Push(CPU->Stack, ROL_C(get_item_from_stack_top(2), get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ARS:
		#else
		INTERP_ARS:
		#endif
			Stack.Push(CPU->Stack, (uint64_t)r3x_ars(get_item_from_stack_top(2), get_item_from_stack_top(1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_PUSHA:
		#else
		INTERP_PUSHA:
		#endif
			Stack.Push(CPU->CallStack, return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_POPA:
		#else
		INTERP_POPA:
		#endif
			Stack.Pop(CPU->CallStack);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Register operations
		#ifndef REX_OPTIMIZE
		case R3X_LOADR:
		#else
		INTERP_LOADR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {	
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer])] = return_32bit_int_from_ip(CPU);
				CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_PUSHR:
		#else
		INTERP_PUSHR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				Stack.Push(CPU->Stack, CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])]);
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			}
			else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_POPR:
		#else
		INTERP_POPR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				uint64_t a = Stack.Pop(CPU->Stack);
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] = a;
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_INCR:
		#else
		INTERP_INCR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] += 1;
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_DECR:
		#else
		INTERP_DECR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] -= 1;
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LOADI:
		#else
		INTERP_LOADI:
		#endif
			CPU->InstructionPointer++;
			set_interrupt(CPU->Memory[CPU->InstructionPointer], CPU);
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_PUSHAR:
		#else
		INTERP_PUSHAR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				Stack.Push(CPU->CallStack, CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])]);
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_POPAR:
		#else
		INTERP_POPAR:
		#endif
			if(CPU->Memory[CPU->InstructionPointer+1] <= MAX_NUMBER_OF_REGISTERS) {
				int a = Stack.Pop(CPU->CallStack);
				CPU->Regs[(CPU->Memory[CPU->InstructionPointer+1])] = a;
				CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			} else {
				printf("Invalid register index\n");
				handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Math Instructions
		#ifndef REX_OPTIMIZE
		case R3X_FSIN:
		#else
		INTERP_FSIN:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(sin(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FCOS:
		#else
		INTERP_FCOS:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(cos(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FTAN:
		#else
		INTERP_FTAN:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(tan(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ASIN:
		#else
		INTERP_ASIN:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(asin(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ACOS:
		#else
		INTERP_ACOS:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(acos(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ATAN:
		#else
		INTERP_ATAN:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(atan(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FSINH:
		#else
		INTERP_FSINH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(sinh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FCOSH:
		#else
		INTERP_FCOSH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(cosh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FTANH:
		#else
		INTERP_FTANH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(tanh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ASINH:
		#else
		INTERP_ASINH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(asinh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ACOSH:
		#else
		INTERP_ACOSH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(acosh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ATANH:
		#else
		INTERP_ATANH:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(atanh(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_CEIL:
		#else
		INTERP_CEIL:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(ceil(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FLOOR:
		#else
		INTERP_FLOOR:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(floor(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_ICONV:
		#else
		INTERP_ICONV:
		#endif
			Stack.Push(CPU->Stack, (uint32_t)(return_float(get_item_from_stack_top(1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FCONV:
		#else
		INTERP_FCONV:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(get_item_from_stack_top(1))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FPOW:
		#else
		INTERP_FPOW:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(pow(return_float(get_item_from_stack_top(2)), return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FABS:
		#else
		INTERP_FABS:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(fabs(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FSQRT:
		#else
		INTERP_FSQRT:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(sqrt(return_float(get_item_from_stack_top(1)))));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Convert to angle
		#ifndef REX_OPTIMIZE
		case R3X_ACONV:
		#else
		INTERP_ACONV:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(get_item_from_stack_top(1))*(180.0/M_PI)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Convert to radian
		#ifndef REX_OPTIMIZE
		case R3X_RCONV:
		#else
		INTERP_RCONV:
		#endif
			Stack.Push(CPU->Stack, return_int_from_float(return_float(get_item_from_stack_top(1))*(M_PI/180.0)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_MOD:
		#else
		INTERP_MOD:
		#endif
			if(get_item_from_stack_top(1) == 0) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_ARITHMETIC);
			} else {
				Stack.Push(CPU->Stack, get_item_from_stack_top(2) % get_item_from_stack_top(1));
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_FMOD:
		#else
		INTERP_FMOD:
		#endif
			if(return_float(get_item_from_stack_top(1)) == 0.0f) {
				handle_cpu_exception(CPU, CPU_EXCEPTION_ARITHMETIC);
			} else {
				Stack.Push(CPU->Stack, return_int_from_float(fmod(return_float(get_item_from_stack_top(2)), return_float(get_item_from_stack_top(1)))));
				CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			}
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		// Epic-ass, first class, sexy error handling
		#ifndef REX_OPTIMIZE
		case R3X_CATCH:
		#else
		INTERP_CATCH:
		#endif
			set_exception_handlers(CPU, get_item_from_stack_top(1), get_item_from_stack_top(2));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_THROW:
		#else
		INTERP_THROW:
		#endif
			handle_cpu_exception(CPU, get_item_from_stack_top(1));
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_HANDLE:
		#else
		INTERP_HANDLE:
		#endif
			CPU->ExceptionFlag = false;
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_BREAK:
		#else
		INTERP_BREAK:
		#endif
			handle_cpu_exception(CPU, CPU_BREAK_POINT);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
				break;
			#else 
				return;
			#endif
		/** RFC Prefix **/
		#ifndef REX_OPTIMIZE
		case RFC_PREFIX:
		#else
		INTERP_RFC:
		#endif
		    CPU->InstructionPointer++;
		    rfc_emulate_instruction(CPU);
		  #ifndef REX_OPTIMIZE
		      break;
		  #else
		      return;
		  #endif
		#ifndef REX_OPTIMIZE
		case R3X_LOR:
		#else
		INTERP_LOR:
		#endif
			if(get_item_from_stack_top(1) == 1 || get_item_from_stack_top(2) == 1) {
				Stack.Push(CPU->Stack, 1);
			} else {
				Stack.Push(CPU->Stack, 0);
			}
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
			break;
			#else
			return;
			#endif
		#ifndef REX_OPTIMIZE
		case R3X_LAND:
		#else
		INTERP_LAND:
		#endif
			if(get_item_from_stack_top(1) == 1 && get_item_from_stack_top(2) == 1) {
				Stack.Push(CPU->Stack, 1);
			} else {
				Stack.Push(CPU->Stack, 0);
			}
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			#ifndef REX_OPTIMIZE
			break;
			#else
			return;
			#endif
		// Exit application
		#ifndef REX_OPTIMIZE
		case R3X_EXIT:
		#else
		INTERP_EXIT:
		#endif
		if(exitcalled == false && CPU->RootDomain->Jobs[CPU->RootDomain->CurrentJobID]!=NULL) {
			if(CPU->RootDomain->Jobs[CPU->RootDomain->CurrentJobID]->ismain == true) {
				// Show exit status
				printf("Program exitted with status: %lu\n", (uint64_t)get_item_from_stack_top(1));		
				r3x_exit_job(CPU->RootDomain, CPU->RootDomain->CurrentJobID);
				exitcalled = true;
			}
			r3x_exit_job(CPU->RootDomain, CPU->RootDomain->CurrentJobID);
		}
			#ifndef REX_OPTIMIZE
			break;
			#else
			return;
			#endif
	#ifndef REX_OPTIMIZE
		default:
			printf("Unknown Opcode: %x, IP: %u\n", (unsigned int)CPU->Memory[CPU->InstructionPointer], CPU->InstructionPointer);
			handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
			break;
	
	}
	#endif
}
static inline uint32_t return_32bit_int(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	register __32bit_typecast temp_typecast32;
	temp_typecast32.__num32 = 0;
	temp_typecast32.__num8.a = a;
	temp_typecast32.__num8.b = b;
	temp_typecast32.__num8.c = c;
	temp_typecast32.__num8.d = d;
	return temp_typecast32.__num32;
}
static inline float return_float(uint32_t num32)
{	
	register __float_typecast temp_typecastFL;;
	temp_typecastFL.float32 = 0.0f;
	temp_typecastFL.__num32 = num32;
	return temp_typecastFL.float32;
}
static inline uint32_t return_int_from_float(float fl32)
{
	register __float_typecast temp_typecastFL;
	temp_typecastFL.float32 = 0.0f;
	temp_typecastFL.__num32 = 0;
	temp_typecastFL.float32 = fl32;
	return temp_typecastFL.__num32;
}
static inline uint32_t return_32bit_int_from_ip(r3x_cpu_t* CPU)
{
	#ifndef R3X_BIG_ENDIAN
	return *((uint32_t*)(&CPU->Memory[CPU->InstructionPointer+1]));
	#else
	return BIG_ENDIAN_INT(*((uint32_t*)(&CPU->Memory[CPU->InstructionPointer+1])));
	#endif
}
static inline void compare_and_set_flag_signed(r3x_cpu_t* CPU, signed int op1, signed int op2)
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
static inline void compare_and_set_flag_unsigned(r3x_cpu_t* CPU, unsigned int op1, unsigned int op2)
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
static inline void set_interrupt(uint8_t interrupt, r3x_cpu_t* CPU) {
	if(return_32bit_int_from_ip(CPU) > CPU->MemorySize) {	
		raise(SIGSEGV);	
	}
	CPU->ISR_handlers[interrupt] = return_32bit_int_from_ip(CPU);
}
static inline void printfstring(char* string, const char * format, ... )
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
static inline void r3x_syscall(r3x_cpu_t* CPU) {
		char buffer[33];
		switch (CPU->Memory[CPU->InstructionPointer+1]){
			case SYSCALL_PUTS:
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) {
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				#ifdef REX_GRAPHICS
				vm_puts(CPU->Graphics->font, (char*)(CPU->Memory + get_item_from_stack_top(1)), CPU->Graphics);	
				#else
				printf("%s", (char*)(CPU->Memory + get_item_from_stack_top(1)));
				#endif
				break;
			case SYSCALL_PUTI:
				 memset(buffer, 0, 33);
				 printfstring(buffer, "%" PRIu64 " ", (uint64_t)get_item_from_stack_top(1));
				 #ifdef REX_GRAPHICS
				 vm_puts(CPU->Graphics->font, buffer, CPU->Graphics);
				 #else
				 printf("%s", buffer);
				 #endif
				 break;
			case SYSCALL_PUTF:
				memset(buffer, 0, 33);
				printfstring(buffer, "%f ", (float)return_float(get_item_from_stack_top(1)));
				#ifdef REX_GRAPHICS
				vm_puts(CPU->Graphics->font, buffer, CPU->Graphics);
				#else
				printf("%s", buffer);
				#endif
				break;
			case SYSCALL_GETC:
				#ifdef REX_GRAPHICS
				// Ensure the there !IS! a keyboard interrupt, our thread will set the is_read value to false if there is.
				if(is_read == false) {
					is_read = true; 
					if(keycode == '\b') { Stack.Push(CPU->Stack, 0x08); } else { Stack.Push(CPU->Stack, keycode); }
				}
				// To prevent race condition, push 0 if is_read == false;
				else {		
					Stack.Push(CPU->Stack, 0);				
				}
				#else 
					(void)CPU;
					char tempchar;
					read(0, &tempchar, 1);
					Stack.Push(CPU->Stack, tempchar);
				#endif
				break;
			case SYSCALL_GLUPDATE:
				#ifdef REX_GRAPHICS
				GLUpdate();
				#endif		
				break;
			case SYSCALL_PUTCH:
				#ifdef REX_GRAPHICS
				(void)CPU;
				char pts[2];
				pts[0] = get_item_from_stack_top(1);
				pts[1] = 0;
				vm_puts(CPU->Graphics->font, pts, CPU->Graphics);	
				#else
				printf("%c", (char)get_item_from_stack_top(1));
				#endif
				break;
			case SYSCALL_ATOI:
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					return;
				}
				Stack.Push(CPU->Stack, atoi((char*)&CPU->Memory[get_item_from_stack_top(1)]));
				break;
			case SYSCALL_ALLOC: 
				/*if((unsigned int)get_item_from_stack_top(1) > 4096) { 
					printf("Attempt to allocate memory more than 4096 bytes at once\n");
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}*/
				Stack.Push(CPU->Stack, CPU->MemorySize);
				unsigned int NumberOfPages = (((unsigned int)get_item_from_stack_top(2) & 0xFFFFF000)+SEGMENT_SIZE) / SEGMENT_SIZE;
				CPU->MemorySize += SEGMENT_SIZE * NumberOfPages;
				CPU->Memory = nt_realloc(CPU->Memory, CPU->MemorySize);
				CPU->CPUMemoryBlocks = RebuildMemoryBlock(CPU->CPUMemoryBlocks, CPU->MemorySize);
				MemoryMap(CPU->CPUMemoryBlocks, RX_RW, CPU->MemorySize - SEGMENT_SIZE*NumberOfPages, CPU->MemorySize);
				break;
			case SYSCALL_DISPATCH:
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize){
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				r3x_dispatch_job(get_item_from_stack_top(1), 1, CPU->RootDomain, true);
				break;
			case SYSCALL_LOADDYNAMIC:
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				Stack.Push(CPU->Stack, load_dynamic_library((char*)(CPU->Memory + get_item_from_stack_top(1)), CPU));			
				break;
			case SYSCALL_OPENSTREAM:
				if((unsigned int)get_item_from_stack_top(1) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				Stack.Push(CPU->Stack, stream_open((char*)(&CPU->Memory[get_item_from_stack_top(1)])));
				break;
			case SYSCALL_CLOSESTREAM:
				stream_close((unsigned int)get_item_from_stack_top(1));
				break;
			case SYSCALL_SEEKSTREAM:
				stream_seek((unsigned int)get_item_from_stack_top(3), (long int)get_item_from_stack_top(2), get_item_from_stack_top(1));
				break;
			case SYSCALL_READSTREAM:
				if((unsigned int)(get_item_from_stack_top(2)+get_item_from_stack_top(1)) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				Stack.Push(CPU->Stack, stream_read((void*)(&CPU->Memory[get_item_from_stack_top(2)]), get_item_from_stack_top(3), get_item_from_stack_top(1))); 
				break;
			case SYSCALL_WRITESTREAM:
				if((unsigned int)(get_item_from_stack_top(2)+get_item_from_stack_top(1)) > CPU->MemorySize) { 
					handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDACCESS);
					// Syscall will increment the instruction by 2 bytes.
					CPU->InstructionPointer -= CPU_INCREMENT_DOUBLE;
					break;
				}
				Stack.Push(CPU->Stack, stream_write((void*)(&CPU->Memory[get_item_from_stack_top(2)]), get_item_from_stack_top(3), get_item_from_stack_top(1))); 
				break;
			case SYSCALL_GETCPUCLOCK:
				(void)NULL;
				struct timespec tp;
				clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
				CPU->CPUClock = (tp.tv_sec * CLOCKS_PER_SEC - CPU->CPUClock);
				Stack.Push(CPU->Stack, return_int_from_float((float)CPU->CPUClock));
				break;
			case SYSCALL_GETCLOCKSPERSEC:
				Stack.Push(CPU->Stack, return_int_from_float((float)CLOCKS_PER_SEC));
				break;
			case SYSCALL_GETARGS:
				Stack.Push(CPU->Stack, ARGUMENT_LOCATION);
				break;
			case SYSCALL_FREE:
				fprintf(stderr, "Freeing unimplemented. Ignoring call.\n");
				break;
			default:
				printf("Invalid Argument passed to syscall\n");
				break;
	}
	return;
}
static inline uint32_t set_bit32(uint32_t num, int bit){
	num |= 1 << bit;
	return num;
}
static inline uint32_t unset_bit32(uint32_t num, int bit){
	num &= ~(1 << bit);
	return num;
}
static inline uint32_t toggle_bit32(uint32_t num, int bit){
	num ^= 1 << bit;
	return num;
}
static inline bool test_bit32(uint32_t num, int bit){
	num = !!(num & (1 << bit));
	if(num == 0){ return false; }
	return true;
}
static inline void push_flags(r3x_cpu_t* CPU){
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
static inline void pop_flags(r3x_cpu_t* CPU){
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
			#ifdef R_DEBUG
			printf("Process %u tried to set an invalid exception handler\n", CPU->RootDomain->CurrentJobID);
			#endif
			return;
	} else {
			#ifdef R_DEBUG
			printf("Process is setting exception handler %u to IP %u\n", ExceptionID, ExceptionHandler);
			#endif
			CPU->ExceptionHandlers[ExceptionID] = ExceptionHandler;
	}
}
void handle_cpu_exception(r3x_cpu_t* CPU, unsigned int ExceptionID){
	if(ExceptionID == CPU_BREAK_POINT) {
		printf("CPU Breakpoint encountered.\n");
		raise(SIGSEGV);
		return;
	}
	if(CPU->ExceptionFlag==true){
		printf("Bad Exception handler, IP: %u. ABORTING...\n", CPU->InstructionPointer);
		raise(SIGSEGV);
		return;
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
			case CPU_EXCEPTION_ARITHMETIC:
				printf("Arithmetic Exception!\n");
				break;
			case CPU_EXCEPTION_EXCEPTION:
				printf("Unhandled exception thrown by program itself\n");
				break;
			default:
				printf("Unknown Exception!\n");
				break;
		}
		
		raise(SIGSEGV);
		return;
	} else {
		Stack.Push(CPU->Stack, CPU->Regs[0]);
		CPU->InstructionPointer = CPU->ExceptionHandlers[ExceptionID];
		CPU->ExceptionFlag = true;
		return;
	}
}
static inline int64_t r3x_ars(int64_t x, int64_t n) {
    if (x < 0 && n > 0)
        return x >> n | ~(~0U >> n);
    else
        return x >> n;
}
#ifdef REX_OPTIMIZE
void* CPUDispatchThread(void* ptr){
	r3x_cpu_t* CPU = (r3x_cpu_t*)ptr;
	while(true){
		if(CPU->InstructionPointer > CPU->MemorySize){
			exitcalled = true;
		}
	}
	return NULL;
}
#endif
