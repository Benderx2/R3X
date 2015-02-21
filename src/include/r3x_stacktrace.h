#ifndef R3X_STACK_TRACE_H
#define R3X_STACK_TRACE_H
#include <system.h>
#ifdef R_DEBUG
static inline void print_backtrace(void);
static inline void print_backtrace(void) {
#ifndef _WIN32
	//! assume posix.
	void * buffer[512];
	const int calls = backtrace(buffer,
		sizeof(buffer) / sizeof(void *));
	backtrace_symbols_fd(buffer, calls, 1);
#else
     //! do extra stuff for win32
     void* stack[100];
     unsigned short frames;
     SYMBOL_INFO* symbol;
     HANDLE process;
     process = GetCurrentProcess();
     SymInitialize(process, NULL, TRUE);
     frames = CaptureStackBackTrace(0, 100, stack, NULL);
     symbol = (SYMBOL_INFO *)calloc( sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
     symbol->MaxNameLen = 255;
     symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
     for(unsigned int i = 0; i < frames; i++) {
         SymFromAddr(process, (uint64_t)(stack[i]), 0, symbol);
	 printf("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
     }
     free(symbol);
#endif
}
#endif
#endif