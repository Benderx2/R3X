#include <r3x_disassemble.h>
void disassemble(uint8_t* input, unsigned int size, FILE* output, char* sectionheader){
	unsigned int i = 0;
	unsigned int domain_num = 0;
	fprintf(output, "%s\n", sectionheader);
	while(i < size){
	switch(input[i]) {
			case R3X_CALL:
				fprintf(output, "call %u ; Call a subroutine\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JMP:
				fprintf(output, "jmp %u ; Load PC with immediate\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JE:
				fprintf(output, "je %u\n",  BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JL:
				fprintf(output, "jl %u\n",  BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JG:
				fprintf(output, "jg %u\n",  BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;	
			case R3X_JZ:
				fprintf(output, "jz %u\n",  BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_PUSH:
				fprintf(output, "push %u\n",  BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_STOSD:
				fprintf(output, "stosd\n");
				i++;
				break;
			case R3X_LODSD:
				fprintf(output, "lodsd\n");
				i++;
				break;
			case R3X_STOSW:
				fprintf(output, "stosw\n");
				i++;
				break;
			case R3X_STOSB:
				fprintf(output, "stosb\n");
				i++;
				break;
			case R3X_LODSB:
				fprintf(output, "lodsb\n");
				i++;
				break;
			case R3X_LODSW:
				fprintf(output, "lodsw\n");
				i++;
				break;
			case R3X_CMPSB:
				fprintf(output, "cmpsb\n");
				i++;
				break;
			case R3X_CMPSD:
				fprintf(output, "cmpsd\n");
				i++;
				break;
			case R3X_CMPSW:
				fprintf(output, "cmpsw\n");
				i++;
				break;
			case R3X_SLEEP:
				fprintf(output, "nop\n");
				i++;
				break;
			case R3X_POP:
				fprintf(output, "pop\n");
				i++;
				break;		
			case R3X_POPA:
				fprintf(output, "popa\n");
				i++;
				break;
			case R3X_ADD:
				fprintf(output, "add\n");
				i++;
				break;
			case R3X_SUB:
				fprintf(output, "sub\n");
				i++;
				break;
			case R3X_DIV:
				fprintf(output, "div\n");
				i++;
				break;
			case R3X_MUL:
				fprintf(output, "mul\n");
				i++;
				break;
			case R3X_FADD:
				fprintf(output, "fadd\n");
				i++;
				break;
			case R3X_FSUB:
				fprintf(output, "fsub\n");
				i++;
				break;
			case R3X_FDIV:
				fprintf(output, "fdiv\n");
				i++;
				break;
			case R3X_FMUL:
				fprintf(output, "fmul\n");
				i++;
				break;
			case R3X_CMP:
				fprintf(output, "cmp\n");
				i++;
				break;
			case R3X_CMPS:
				fprintf(output, "cmps\n");
				i++;
				break;
			case R3X_DUP:
				fprintf(output, "dup\n");
				i++;
				break;
			case R3X_LOADLIB:
				fprintf(output, "loadlib\n");
				i++;
				break;
			case R3X_LIBEXEC:
				fprintf(output, "libexec\n");
				i++;
				break;
			case R3X_MEMCPY:
				fprintf(output, "memcpy\n");
				i++;
				break;
			case R3X_RET:
				fprintf(output, "ret\n");
				i++;
				break;
			case R3X_AND:
				fprintf(output, "and\n");
				i++;
				break;
			case R3X_XOR:
				fprintf(output, "xor\n");
				i++;
				break;
			case R3X_OR:
				fprintf(output, "or\n");
				i++;
				break;
			case R3X_NOT:
				fprintf(output, "not\n");
				i++;
				break;
			case R3X_NEG:	
				fprintf(output, "neg\n");
				i++;
				break;
			case R3X_SHR:
				fprintf(output, "shr\n");
				i++;
				break;
			case R3X_SHL:
				fprintf(output, "shl\n");
				i++;
				break;
			case R3X_FSIN:
				fprintf(output, "fsin\n");
				i++;
				break;
			case R3X_FCOS:
				fprintf(output, "fcos\n");
				i++;
				break;
			case R3X_FTAN:
				fprintf(output, "ftan\n");
				i++;
				break;
			case R3X_ASIN:
				fprintf(output, "asin\n");
				i++;
				break;
			case R3X_ACOS:
				fprintf(output, "acos\n");
				i++;
				break;
			case R3X_ATAN:
				fprintf(output, "atan\n");
				i++;
				break;
			case R3X_MOD:
				fprintf(output, "mod\n");
				i++;
				break;
			case R3X_FMOD:
				fprintf(output, "fmod\n");
				i++;
				break;
			case R3X_ACONV:
				fprintf(output, "aconv\n");
				i++;
				break;
			case R3X_RCONV:
				fprintf(output, "rconv\n");
				i++;
				break;
			case R3X_EXIT:
				fprintf(output, "exit\n");
				domain_num++;
				i++;
				break;
			case R3X_PUSHA:
				fprintf(output, "pusha %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_LOADI:
				fprintf(output, "loadi %u, %u\n", (*((uint8_t*)&input[i+1])),BYTE_SWAP(*((uint32_t*)&input[i+2])) );
				i += 6;
				break;
			case R3X_LOADR:
				fprintf(output, "loadr R%u, %u\n", (*((uint8_t*)&input[i+1])),BYTE_SWAP(*((uint32_t*)&input[i+2])) );
				i += 6;
				break;
			case R3X_PUSHR:
				fprintf(output, "pushr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_POPR:
				fprintf(output, "popr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_PUSHAR:
				fprintf(output, "pushar R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_POPAR:
				fprintf(output, "popar R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_INT:
				fprintf(output, "int 0x%x\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_SYSCALL:
				fprintf(output, "syscall 0x%x\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_INCR:
				fprintf(output, "incr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_DECR:
				fprintf(output, "decr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_CATCH:
				fprintf(output, "catch\n");
				i += 1;
				break;
			case R3X_HANDLE:
				fprintf(output, "handle\n");
				i += 1;
				break;
			case R3X_THROW:
				fprintf(output, "throw\n");
				i += 1;
				break;
			case R3X_ROL:
				fprintf(output, "rol\n");
				i += 1;
				break;
			case R3X_ROR:
				fprintf(output, "ror\n");
				i += 1;
				break;
			case R3X_TERN:
				fprintf(output, "tern\n");
				i += 1;
				break;
			case R3X_LOADS:
				fprintf(output, "loads %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_STORES:
				fprintf(output, "stores %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_LOADSR:
				fprintf(output, "loadsr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_STORESR:
				fprintf(output, "storesr R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_PUSHF:
				fprintf(output, "pushf\n");
				i += 1;
				break;
			case R3X_POPF:
				fprintf(output, "popf\n");
				i += 1;
				break;
			case R3X_CALLDYNAMIC:
				fprintf(output, "calldynamic\n");
				i += 1;
				break;
			case R3X_POPN:
				fprintf(output, "popn %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_SETE:
				fprintf(output, "sete R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_SETNE:
				fprintf(output, "setne R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_SETG:
				fprintf(output, "setg R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_SETL:
				fprintf(output, "setl R%u\n", (uint8_t)input[i+1]);
				i += 2;
				break;
			case R3X_FPOW:
				fprintf(output, "fpow\n");
				i += 1;
				break;
			case R3X_FSINH:
				fprintf(output, "fsinh\n");
				i++;
				break;
			case R3X_FCOSH:
				fprintf(output, "fcosh\n");
				i++;
				break;
			case R3X_FTANH:
				fprintf(output, "ftanh\n");
				i++;
				break;
			case R3X_ASINH:
				fprintf(output, "asinh\n");
				i++;
				break;
			case R3X_ACOSH:
				fprintf(output, "acosh\n");
				i++;
				break;
			case R3X_ATANH:
				fprintf(output, "atanh\n");
				i++;
				break;
			case R3X_FLOOR:
				fprintf(output, "floor\n");
				i++;
				break;
			case R3X_CEIL:
				fprintf(output, "ceil\n");
				i++;
				break;
			case R3X_ICONV:
				fprintf(output, "iconv\n");
				i++;
				break;
			case R3X_FCONV:
				fprintf(output, "fconv\n");
				i++;
				break;
			case R3X_FABS:
				fprintf(output, "fabs\n");
				i++;
				break;
			case R3X_FSQRT:
				fprintf(output, "fsqrt\n");
				break;
			case R3X_JMPL:
				fprintf(output, "jmpl %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JLL:
				fprintf(output, "jll %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JGL:
				fprintf(output, "jgl %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JZL:
				fprintf(output, "jzl %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_JEL:
				fprintf(output, "jel %u\n", BYTE_SWAP(*((uint32_t*)&input[i+1])));
				i += 5;
				break;
			case R3X_PUSHIP:
				fprintf(output, "puship\n");
				i++;
				break;
			default:
				fprintf(output, "; Opcode Not recognized\n db %u\n", (uint8_t)input[i]);
				i++;
				break;
		}
	}
	fprintf(output, "Disassembling Complete");
}
