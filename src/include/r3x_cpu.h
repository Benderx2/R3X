#ifndef __R3X_CPU_H
#define __R3X_CPU_H
#include <stdint.h>
#include <stdbool.h>
#include <r3x_format.h>
#include <r3x_stack.h>
#include <r3x_graphics.h>
#include <r3x_object.h>
#include <time.h>
#define CPU_INVALID_OPCODE_SIGNAL -1
#define CPU_EXIT_SIGNAL -2
#define CPU_INCREMENT_SINGLE 1
#define CPU_INCREMENT_DOUBLE 2
#define CPU_INCREMENT_WITH_32_OP 5
#define CPU_INCREMENT_WITH_32_OP_2 9
#define CPU_INCREMENT_WITH_32_OP_3 13
#define R_LOCK 0
#define R_SHUTDOWN 1
#define R_KEYPRESS 2  
#define R_TIMER 3
typedef struct r3x_job {
	uint32_t InstructionPointer;
	vstack_t* Stack;
	vstack_t* CallStack;
	int Regs[5];
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
typedef struct r3x_cpu {
	uint8_t* Memory;
	uint32_t ISR_handlers[256];
	vstack_t* Stack;
	vstack_t* CallStack;
	Graphics_t* Graphics;
	unsigned int InstructionPointer;
	int ErrorCode;
	bool EqualFlag;
	bool GreaterFlag;
	bool LesserFlag;
	bool ZeroFlag;	
	int Regs[5];
	unsigned int CurrentInstruction;
	unsigned int MemorySize;
	unsigned int CurrentJobID;
	r3x_global_domain_t* RootDomain;
	ObjectList_t* ObjectList;
} r3x_cpu_t;
#include <r3x_dispatcher.h>
extern r3x_cpu_t* r3_cpu;
int r3x_cpu_loop(r3x_cpu_t* CPU, r3x_header_t* header);
#endif
