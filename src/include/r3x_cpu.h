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
#ifndef R3X_CPU_H
#define R3X_CPU_H
#include <system.h>
#include <r3x_format.h>
#include <r3x_stack.h>
#include <r3x_graphics.h>
#include <r3x_object.h>
#include <r3x_memory.h>
#define EFLAG_BIT 0
#define GFLAG_BIT 1
#define LFLAG_BIT 2
#define ZFLAG_BIT 3
#define EXFLAG_BIT 4
#define R3X_FALSE 0
#define BIOS_START 0
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
#define CPU_BREAK_POINT 4
#define R_LOCK 0
#define R_SHUTDOWN 1
#define R_KEYPRESS 2  
#define R_TIMER 3
#define MAX_NUMBER_OF_REGISTERS 21
#define TOTAL_EXCEPTIONS 3
#define BP_REG 21
typedef struct r3x_job {
	uint32_t InstructionPointer;
	vstack_t* Stack;
	vstack_t* CallStack;
	uint32_t Regs[MAX_NUMBER_OF_REGISTERS+1];
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
	bool Alive;
} r3x_job_t;
typedef struct r3x_global_domain {
	r3x_job_t** Jobs;
	unsigned int CurrentJobID;
	unsigned int NumberOfActiveJobs;
	unsigned int TotalNumberOfJobs;
} r3x_global_domain_t;
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
	uint64_t Regs[MAX_NUMBER_OF_REGISTERS+1];
	uint32_t ExceptionHandlers[4];
	// Global -- Thread Independent.
	r3x_memory_blocks* CPUMemoryBlocks;
	double CPUClock;
	uint8_t* Memory;
	uint32_t ISR_handlers[256];
	unsigned int CurrentInstruction;
	unsigned int MemorySize;
	unsigned int HeapAddr;
	bool use_frequency;
	double CPUFrequency;
	r3x_global_domain_t* RootDomain;
	Graphics_t* Graphics;
	ObjectList_t* ObjectList;
} r3x_cpu_t;
#include <r3x_dispatcher.h>
extern r3x_cpu_t* r3_cpu;
int r3x_cpu_loop(r3x_cpu_t* CPU, r3x_header_t* header);
#endif
