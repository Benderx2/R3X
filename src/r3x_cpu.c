#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <r3x_format.h>
#include <r3x_cpu.h>
#include <r3x_opcodes.h>
#include <r3x_stack.h>
#include <r3x_native.h>
#ifdef REX_GRAPHICS
#include <r3x_graphics.h>
#endif
#include <r3x_exception.h>
#include <nt_malloc.h>
bool exitcalled = false;
int r3x_emulate_instruction(r3x_cpu_t* CPU);
uint32_t return_32bit_int(uint8_t, uint8_t, uint8_t, uint8_t);
uint32_t return_32bit_int_from_ip(r3x_cpu_t*);
float return_float(uint32_t num32);
int keyboard_thread(void* data);
uint32_t return_int_from_float(float fl32);
void printfstring(char* string, const char * format, ... );
void compare_and_set_flag(r3x_cpu_t* CPU, int op1, int op2);
void set_interrupt(uint8_t interrupt, r3x_cpu_t* CPU);
typedef union __32bit_typecast {
	uint32_t __num32;
	struct {
		#ifdef LITTLE_ENDIAN
		uint8_t a, b, c, d;
		#else
		uint8_t d, c, b, a;
	 	#endif
	} __num8;
} __32bit_typecast;
typedef union __float_typecast {
	uint32_t __num32;
	float float32;
} __float_typecast;
__32bit_typecast temp_typecast32;
__float_typecast temp_typecastFL;
bool is_read = true;
char keycode = 0;
int code = 0;
SDL_Event key_event = {SDL_USEREVENT};
int r3x_cpu_loop(r3x_cpu_t* CPU, r3x_header_t* header)
{
	temp_typecast32.__num8.a = 0;
	temp_typecast32.__num8.b = 0;
	temp_typecast32.__num8.c = 0;
	temp_typecast32.__num8.d = 0;
	r3x_dispatch_job(header->r3x_init, 1, CPU->RootDomain, true);
	// Initialise keyboard thread.
	#ifdef REX_GRAPHICS
	SDL_Thread *kthread = NULL;
	kthread = SDL_CreateThread(keyboard_thread, NULL );
	#endif
	code = 0;
	while (CPU->InstructionPointer < header->r3x_init + header->r3x_text_size && code != CPU_EXIT_SIGNAL && code != CPU_INVALID_OPCODE_SIGNAL){
			if(exitcalled == true) { 
				break;
			}
			int i = r3x_load_job_state(CPU, CPU->RootDomain);
			if(i != -1) {
				code = r3x_emulate_instruction(CPU);
				r3x_save_job_state(CPU, CPU->RootDomain);
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
		case R3X_SLEEP:
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_PUSH:
			Stack.Push(CPU->Stack, return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_POP:
			Stack.Pop(CPU->Stack);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ADD:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_SUB:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) - Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_MUL:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) * Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_DIV:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2) / Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
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
		case R3X_CMP:
			compare_and_set_flag(CPU, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 2), Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - 1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
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
		case R3X_SYSCALL:
			if (CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTS){
				if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize) {
					raise(SIGSEGV);
				}
				#ifdef REX_GRAPHICS
				vm_puts(CPU->Graphics->font, (char*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)), CPU->Graphics);	
				#else
				printf("%s", (char*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)));
				#endif
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTI){
				 char buffer[33];
				 memset(buffer, 0, 33);
				 printfstring(buffer, "%d", Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
				 #ifdef REX_GRAPHICS
				 vm_puts(CPU->Graphics->font, buffer, CPU->Graphics);
				 #else
				 printf("%s", buffer);
				 #endif

			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_PUTF){
				char buffer[33];
				memset(buffer, 0, 33);
				printfstring(buffer, "%f", (float)return_float(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)));
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
				vm_putc((char)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1), CPU->Graphics);
				gl_text_update(CPU->Graphics);			
				#else
				printf("%c", (char)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
				#endif
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_ATOI) {
				if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize) { 
					raise(SIGSEGV);
				}
				Stack.Push(CPU->Stack, atoi((char*)&CPU->Memory[Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)]));			
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_ALLOC) { 
				if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > 4096) { 
					printf("Attempt to allocate memory more than 4096 bytes at once\n");
					raise(SIGSEGV);
				}	
				CPU->MemorySize += Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1);
				CPU->Memory = nt_realloc(CPU->Memory, CPU->MemorySize);
			}
			else if(CPU->Memory[CPU->InstructionPointer+1] == SYSCALL_DISPATCH) {
				if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize){
					raise(SIGSEGV);
				}
				r3x_dispatch_job(Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1), 1, CPU->RootDomain, true);
							
			}
			else {
				printf("Invalid Argument passed to R3X_SYSCALL\n");
			}
			CPU->InstructionPointer += CPU_INCREMENT_DOUBLE;
			break;
		case R3X_LOADS:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack - return_32bit_int_from_ip(CPU)));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_LOAD:
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize){ 
				raise(SIGSEGV);			
			}
			Stack.Push(CPU->Stack, *(uint32_t*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STORE:
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2) > CPU->MemorySize){ 
				raise(SIGSEGV);			
			}
			*((uint32_t*)&(CPU->Memory[Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2)])) = Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1);
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LODSB:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				raise(SIGSEGV);			
			}
			CPU->Regs[1] = CPU->Memory[CPU->Regs[0]];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LODSD:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				raise(SIGSEGV);			
			}
			CPU->Regs[1] = *((uint32_t*)(&CPU->Memory[CPU->Regs[0]]));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STOSB:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize){ 
				raise(SIGSEGV);			
			}
			CPU->Memory[CPU->Regs[0]] = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_STOSD:
			if((unsigned int)CPU->Regs[0] > CPU->MemorySize) {
				raise(SIGSEGV);
			}
			*((uint32_t*)(&CPU->Memory[CPU->Regs[0]])) = CPU->Regs[1];
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_AND:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) & return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_OR:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) | return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_XOR:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) ^ return_32bit_int_from_ip(CPU));
			CPU->InstructionPointer += CPU_INCREMENT_WITH_32_OP;
			break;
		case R3X_DUP:
			Stack.Push(CPU->Stack, Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LOADLIB:
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize) { 
				raise(SIGSEGV);			
			}
			#ifdef REX_DYNAMIC
				load_native_library((char*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)), CPU);
			#else
				printf("Not compiled with dynamic library support. Attempt to call LOADLIB, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_LIBEXEC:
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize) { 
				raise(SIGSEGV);			
			}
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2) > CPU->MemorySize) { 
				raise(SIGSEGV);			
			}
			#ifdef REX_DYNAMIC
			Stack.Push(CPU->Stack, native_call((char*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1)), returnhandle((char*)(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2)))));
			#else
			printf("Not compiled with dynamic library support. Attempt to call LIBEXEC, not supported\n");
			#endif
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
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
		case R3X_MEMCPY:
			if((unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-3)+(unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize || (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2) + (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1) > CPU->MemorySize) {
					raise(SIGSEGV);			
			}
			memcpy(CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-3), CPU->Memory + Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2), Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
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
		case R3X_NOT: 
			Stack.Push(CPU->Stack, ~Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_NEG:
			Stack.Push(CPU->Stack, !Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
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
		// Always set your right foot first soldier! #weirdsuperstitions
		case R3X_SHR:
			Stack.Push(CPU->Stack, (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2) >> (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_SHL:
			Stack.Push(CPU->Stack, (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-2) << (unsigned int)Stack.GetItem(CPU->Stack, CPU->Stack->top_of_stack-1));
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ROR:
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_ROL:
			CPU->InstructionPointer += CPU_INCREMENT_SINGLE;
			break;
		case R3X_EXIT:
			if(CPU->RootDomain->Jobs[CPU->RootDomain->CurrentJobID]->ismain == true) {		
				r3x_exit_job(CPU->RootDomain);
				return -2; // Main exitted close everything and return...
			}
			r3x_exit_job(CPU->RootDomain);
			return 0;
		default:
			printf("Unknown Opcode: %x, IP: %u\n", (unsigned int)CPU->Memory[CPU->InstructionPointer], CPU->InstructionPointer);
			raise(SIGSEGV);
	}
	return 0;
}
uint32_t return_32bit_int(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	temp_typecast32.__num32 = 0;
	temp_typecast32.__num8.a = a;
	temp_typecast32.__num8.b = b;
	temp_typecast32.__num8.c = c;
	temp_typecast32.__num8.d = d;
	return temp_typecast32.__num32;
}
float return_float(uint32_t num32)
{
	temp_typecastFL.float32 = 0.0f;
	temp_typecastFL.__num32 = num32;
	return temp_typecastFL.float32;
}
uint32_t return_int_from_float(float fl32)
{
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
void compare_and_set_flag(r3x_cpu_t* CPU, int op1, int op2)
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
