#ifndef REX_VIRTUAL_MACHINE_H
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <stdarg.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <math.h>
#include <time.h>
#define CPU_INVALID_OPCODE_SIGNAL -1
#define CPU_EXIT_SIGNAL -2
#define CPU_INCREMENT_SINGLE 1
#define CPU_INCREMENT_DOUBLE 2
#define CPU_INCREMENT_WITH_32_OP 5
#define CPU_INCREMENT_WITH_32_OP_2 9
#define CPU_INCREMENT_WITH_32_OP_3 13
#define CPU_EXCEPTION_INVALIDACCESS 0
#define CPU_EXCEPTION_INVALIDOPCODE 1
#define CPU_EXCEPTION_EXCEPTION 2
#define CPU_EXCEPTION_ARITHMETIC 3
#define MAX_NUMBER_OF_REGISTERS 20
#define TOTAL_EXCEPTIONS 3
#define R_LOCK 0
#define R_SHUTDOWN 1
#define R_KEYPRESS 2  
#define R_TIMER 3
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
typedef struct r3x_job {
	uint32_t InstructionPointer;
	vstack_t* Stack;
	vstack_t* CallStack;
	uint32_t Regs[21];
	uint32_t ExceptionHandlers[4];
	bool EqualFlag;
	bool GreaterFlag;
	bool LesserFlag;
	bool ZeroFlag;
	bool ExceptionFlag;
	int CycleUpdate;
	int JobID;
	bool ismain;
	bool paused;
} r3x_job_t;
typedef struct r3x_global_domain {
	r3x_job_t** Jobs;
	unsigned int CurrentJobID;
	unsigned int NumberOfActiveJobs;
	unsigned int TotalNumberOfJobs;
} r3x_global_domain_t;
typedef struct {
	void* MemoryBlocks;
} r3x_memory_blocks;
typedef struct r3x_cpu {
	// Not global -- Thread dependent
	vstack_t* Stack;
	vstack_t* CallStack;
	unsigned int InstructionPointer;
	int ErrorCode;
	bool EqualFlag;
	bool GreaterFlag;
	bool LesserFlag;
	bool ZeroFlag;
	bool ExceptionFlag;
	uint32_t FLAGS;
	uint32_t Regs[21];
	uint32_t ExceptionHandlers[4];
	// Global -- Thread Independent.
	r3x_memory_blocks CPUMemoryBlocks;
	double CPUClock;
	uint8_t* Memory;
	uint32_t ISR_handlers[256];
	unsigned int CurrentInstruction;
	unsigned int MemorySize;
	unsigned int HeapAddr;
	bool use_frequency;
	double CPUFrequency;
	r3x_global_domain_t* RootDomain;
	void* Graphics;
	void* ObjectList;
} r3x_cpu_t;
extern stack_construct Stack;
#endif
