#include <r3x_exception.h>
#include <r3x_version.h>
#include <nt_malloc.h>
#include <unistd.h>
extern r3x_cpu_t* r3_cpu;
char str[81];
char str2[81];
void printstatus(void);
void debugger(void);
void REX_EXCEPTION_HANDLER(int SIGNUM) {
	(void)SIGNUM;
	printf("Exception Detected! Debugger ONLINE.\n");
	debugger();
}
void debugger(void) { 
	printstatus();
	printf("Welcome to REX Debugger, based upon FVM Technology. %s.\nType 'help' for help.", R3X_SYSTEM_VERSION);
	// go into debugger 
	(void)a1; (void)a2;
	while(true) { 
		memset(str, 0, 80);
		unsigned int a1 = 0; unsigned int a2 = 0;
		printf("\n>:");
		char* assassassassassassass = fgets(str, 80, stdin); // gcc warns for ignoring return values, my ass.
		(void)assassassassassassass;
		if(strncmp(str, "help", 4) == 0) { 
			printf("REX Debugger -- help\n");
			printf("Commands: \n");
			printf("status --- Print information about program, CPU and Stack\n");
			printf("memprobe x -- prints a 32-bit integer @ x address in VM memory\n");
			printf("setip x -- Sets an Instruction Pointer\n");
			printf("setreg 1/2/3/4 x - Sets a register to value x\n");
			printf("push x -- Pushes value x to stack\n");
			printf("pop -- Pops from Stack\n");
			printf("continue -- Continues execution\n");
			printf("quit -- Exits the VM and debugger");
		}
		else if(strncmp(str, "quit", 4) == 0) {
			exit(EXIT_FAILURE);				
		}
		else if(strncmp(str, "memprobe", 8) == 0){ 
			memcpy(&str2, (&str[0] + 9), 70);
			a1 = atoi(&str2[0]);
			if(a1 > r3_cpu->MemorySize) { 
				printf("Cannot probe memory, invalid address\n");
			}	
			else {
				printf("Hex: %x\nDecimal: %u\nAt Address: %d", *(uint32_t*)(&r3_cpu->Memory[a1]), *(uint32_t*)(&r3_cpu->Memory[a1]), a1);	
				a1 = 0;		
			}
		} 
		else if(strncmp(str, "setip", 5) == 0) {
			memcpy(&str2, (&str[0] + 6), 70);
			a1 = atoi(&str2[0]);	
			if(a1 > r3_cpu->MemorySize) { 
				printf("Invalid Instruction Pointer");			
			}
			r3_cpu->InstructionPointer = a1;
		}
		else if(strncmp(str, "push", 4) == 0) {
			memcpy(&str2, (&str[0] + 5), 70);
			a1 = atoi(&str2[0]);	
			Stack.Push(r3_cpu->Stack, a1);
			printf("Top of Stack: %u\nStack Size: %u", r3_cpu->Stack->top_of_stack, r3_cpu->Stack->stack_count);	
		}
		else if(strncmp(str, "pop", 3) == 0) { 
			printf("Popped from Stack : %u", Stack.Pop(r3_cpu->Stack));		
		}
		else if(strncmp(str, "setreg", 6) == 0) { 
			char* token = strtok(str, " ");
			(void)token;
			a1 = 0; a2 = 0;
			a1 = atoi(strtok(NULL, " "));
			a2 = atoi(strtok(NULL, " "));	
			if(a1 > 4) { 
				printf("Invalid Register\n");
			}
			else { 
				r3_cpu->Regs[a1] = a2;			
			}
		}
		else if(strncmp(str, "status", 6) == 0) { 
			printstatus();		
		}
		else if(strncmp(str, "continue", 8) == 0) {
			return;		
		}
		else { 
			printf("Invalid command. Type 'help' for help");		
		}
	}
}
void printstatus(void) { 
	printf("Preparing Debugging Info.\nProgram backtrace:\n");
	void * buffer[255];
	const int calls = backtrace(buffer,
		sizeof(buffer) / sizeof(void *));
	backtrace_symbols_fd(buffer, calls, 1);
	assert(r3_cpu);
	assert(r3_cpu->Stack);
	assert(r3_cpu->CallStack);
	assert(r3_cpu->Memory);
	printf("Generating CPU report..\n");
	printf("|Stack Information| \n");
	printf("Stack (Address) : %u\n", (unsigned int)((intptr_t)r3_cpu->Stack));
	printf("Stack Size : %u\n", (unsigned int)r3_cpu->Stack->stack_count);
	printf("Top of Stack : %u\n", (unsigned int)r3_cpu->Stack->top_of_stack);
	printf("Last pushed value : %u\n", (unsigned int)Stack.GetItem(r3_cpu->Stack, r3_cpu->Stack->top_of_stack-1));
	printf("Last popped value : %u\n", (unsigned int)Stack.GetItem(r3_cpu->Stack, r3_cpu->Stack->top_of_stack));
	printf("|Call Stack Information|\n");
	printf("Call Stack (Address) : %u\n", (unsigned int)((intptr_t)r3_cpu->CallStack));
	printf("Last pushed Value : %u\n", (unsigned int)Stack.GetItem(r3_cpu->CallStack, r3_cpu->CallStack->top_of_stack-1));
	printf("Last popped Value : %u\n", (unsigned int)Stack.GetItem(r3_cpu->CallStack, r3_cpu->CallStack->top_of_stack-1));
	printf("|CPU Information|\n");
	printf("Memory (Address of Buffer): %p\n", (void*)r3_cpu->Memory);
	printf("Memory Size: %u\n", (unsigned int)r3_cpu->MemorySize);
	printf("Instruction Pointer: %u\n", (unsigned int)r3_cpu->InstructionPointer);
	printf("-->|CPU Registers|\n");
	printf("R0: %d | R1 : %d | R2 : %d | R3 : %d | R4 : %d |\n", r3_cpu->Regs[0], r3_cpu->Regs[1], r3_cpu->Regs[2], r3_cpu->Regs[3], r3_cpu->Regs[4]);
	printf("-->|CPU Flags|\n");
	printf("Equal: %s, Zero: %s, Greater: %s, Lesser: %s\n", r3_cpu->EqualFlag ? "true" : "false", r3_cpu->ZeroFlag ? "true" : "false", r3_cpu->GreaterFlag ? "true" : "false", r3_cpu->LesserFlag ? "true" : "false");
	if(r3_cpu->InstructionPointer > r3_cpu->MemorySize) {
		printf("Exception: Instruction Pointer overflow\n");
	}
	else {
		printf("Last Instruction Executed [OPCODE] : %x\n", r3_cpu->CurrentInstruction);	
	}
	printf("Log complete.\n");
}
