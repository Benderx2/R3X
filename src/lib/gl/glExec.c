#include "virtual-machine.h"
#include ""
/* gl implementation for r3x */

r3x_cpu_t* CPU = NULL;

typedef struct GL_EXECUTION_INSTANCE {
	unsigned char* Assembly;
	unsigned char* Name;
	uint32_t id;
	int32_t(*GLCompiledFunction)(void);
} GLExecutionInstance;
uint32_t Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
}
uint32_t gl_CreateInstance(void) {
	//! get name
	unsigned char* name = (char*)GetArgument(CPU, 1, 1);
} 
