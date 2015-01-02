#include <r3x_exception.h>
#include <r3x_dispatcher.h>
#include <r3x_version.h>
#include <nt_malloc.h>
#include <unistd.h>
extern r3x_cpu_t* r3_cpu;
#define MAX_INPUT_LEN 80
char str[MAX_INPUT_LEN+1];
char str2[ MAX_INPUT_LEN+1];
void printstatus(void);
void printregstatus(void);
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
	while(true) { 
		memset(str, 0, MAX_INPUT_LEN+1);
		unsigned int a1 = 0; unsigned int a2 = 0;
		(void)a1; (void)a2;
		printf("\n$ ");
		char* input = fgets(str, MAX_INPUT_LEN, stdin); // gcc warns for ignoring return values, my ass.
		if(strncmp(input, "help", 4) == 0) { 
			printf("REX Debugger -- help\n");
			printf("Commands: \n");
			printf("status --- Print information about program, CPU and Stack\n");	
			printf("regstatus -- Show register status\n");
			printf("switchdomain x -- Switches domains (processes)\n"); 
			printf("memprobe x -- prints a 32-bit integer @ x address in VM memory\n");
			printf("setip x -- Sets an Instruction Pointer\n");
			printf("setreg 1-20 x - Sets a register to value x\n");
			printf("push x -- Pushes value x to stack\n");
			printf("pop -- Pops from Stack\n");
			printf("continue -- Continues execution\n");
			printf("quit -- Exits the VM and debugger");
		}
		else if(strncmp(input, "quit", 4) == 0) {
			exit(EXIT_FAILURE);				
		}
		else if(strncmp(input, "memprobe", 8) == 0){ 
			memcpy(&str2, (&str[0] + 9),  MAX_INPUT_LEN-10);
			a1 = atoi(&str2[0]);
			if(a1 > r3_cpu->MemorySize) { 
				printf("Cannot probe memory, invalid address\n");
			}	
			else {
				printf("Hex: %x\nDecimal: %u\nAt Address: %d", *(uint32_t*)(&r3_cpu->Memory[a1]), *(uint32_t*)(&r3_cpu->Memory[a1]), a1);	
				a1 = 0;		
			}
		} 
		else if(strncmp(input, "setip", 5) == 0) {
			memcpy(&str2, (&str[0] + 6), 70);
			a1 = atoi(&str2[0]);	
			if(a1 > r3_cpu->MemorySize) { 
				printf("Invalid Instruction Pointer");			
			}
			r3_cpu->InstructionPointer = a1;
		}
		else if(strncmp(input, "push", 4) == 0) {
			memcpy(&str2, (&str[0] + 5), 70);
			a1 = atoi(&str2[0]);	
			Stack.Push(r3_cpu->Stack, a1);
			printf("Top of Stack: %u\nStack Size: %u", r3_cpu->Stack->top_of_stack, r3_cpu->Stack->stack_count);	
		}
		else if(strncmp(input, "pop", 3) == 0) { 
			printf("Popped from Stack : %u", Stack.Pop(r3_cpu->Stack));		
		}
		else if(strncmp(str, "setreg", 6) == 0) { 
			char* token = strtok(input, " ");
			(void)token;
			a1 = 0; a2 = 0;
			a1 = atoi(strtok(NULL, " "));
			a2 = atoi(strtok(NULL, " "));	
			if(a1 > MAX_NUMBER_OF_REGISTERS) { 
				printf("Invalid Register\n");
			}
			else { 
				r3_cpu->Regs[a1] = a2;			
			}
		}
		else if(strncmp(input, "status", 6) == 0) { 
			printstatus();		
		}
		else if(strncmp(input, "switchdomain", 12) == 0) {
			a1 = 0;
			char* token = strtok(input, " "); 
			(void)token;
			int savedomainID = r3_cpu->RootDomain->CurrentJobID;
			a1 = atoi(strtok(NULL, " "));
			if(r3x_load_job_state(r3_cpu, r3_cpu->RootDomain, a1)==-1){
				printf("Error: Domain does not exist");
				r3_cpu->RootDomain->CurrentJobID = savedomainID;
				r3x_load_job_state(r3_cpu, r3_cpu->RootDomain, savedomainID);
			}
			else {
				r3_cpu->RootDomain->CurrentJobID = a1;
				printf("Switched to domain: %u", (unsigned int)r3_cpu->RootDomain->CurrentJobID);
			}
		}
		else if(strncmp(input, "regstatus", 9) == 0) { 
			printregstatus();
		}
		else if(strncmp(input, "continue", 8) == 0) {
			return;		
		} else if(strncmp(input, "\n", 1)==0){
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
	printf("Type 'regstatus' for values for registers\n");
	printf("-->|CPU Flags|\n");
	printf("Equal: %s, Zero: %s, Greater: %s, Lesser: %s\n", r3_cpu->EqualFlag ? "true" : "false", r3_cpu->ZeroFlag ? "true" : "false", r3_cpu->GreaterFlag ? "true" : "false", r3_cpu->LesserFlag ? "true" : "false");
	printf("Current Domain ID: %u\n", r3_cpu->RootDomain->CurrentJobID);
	if(r3_cpu->InstructionPointer > r3_cpu->MemorySize) {
		printf("Exception: Instruction Pointer overflow\n");
	}
	else {
		printf("Last Instruction Executed [OPCODE] : %x\n", r3_cpu->CurrentInstruction);	
	}
	printf("Log complete.\n");
}
void printregstatus(void) { 
	printf("Values for Current Domain:\n"); 
	for(unsigned int i = 0; i <= MAX_NUMBER_OF_REGISTERS; i++) {
		printf("R%u: %u\t", i, (unsigned int)r3_cpu->Regs[i]);	
	}
}
