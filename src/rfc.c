/**
 * This file contains the interpreter for the RFC (REX Fast computing) extension.
 * Executed when an RFC Prefix (0x8C) is encountered.
**/
#include <r3x_cpu.h>
#include <r3x_opcodes.h>
#include <big_endian.h>

static inline uint64_t read_64bit_int_from_instruction_pointer(r3x_cpu_t* CPU);

uint64_t rfc_emulate_instruction(r3x_cpu_t* CPU) {
    switch(CPU->Memory[CPU->InstructionPointer]) {
      case RFC_LOADR64:
	CPU->InstructionPointer++;
	if(CPU->Memory[CPU->InstructionPointer] > MAX_NUMBER_OF_REGISTERS) {
	    handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
	    break;
	} else {
	  CPU->Regs[CPU->Memory[CPU->InstructionPointer]] = read_64bit_int_from_instruction_pointer(CPU);
	  CPU->InstructionPointer += CPU_INCREMENT_WITH_64_OP;
	}
	break;
      case RFC_PUSH64:
	Stack.Push(CPU->Stack, read_64bit_int_from_instruction_pointer(CPU));
	CPU->InstructionPointer += CPU_INCREMENT_WITH_64_OP;
	break;
      default:
	handle_cpu_exception(CPU, CPU_EXCEPTION_INVALIDOPCODE);
	break;
    }
    return 0;
}
static inline uint64_t read_64bit_int_from_instruction_pointer(r3x_cpu_t* CPU) {
    return BIG_ENDIAN_INT(*((uint64_t*)(&CPU->Memory[CPU->InstructionPointer+1])));
}