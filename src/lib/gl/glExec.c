#define REX_GRAPHICS
#define GL_TRUE 0
#define GL_FALSE 1
#define SO_NAME "glExec.so"

#include <virtual-machine.h>
#include <nt_malloc.h>

/* gl implementation for r3x */

r3x_cpu_t* CPU = NULL;
void Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
	nt_malloc_init(false);
	GLInstances = nt_malloc(sizeof(GLExecutionInstances));
	GLInstances->Instances = nt_malloc(sizeof(GLExecutionInstance)*16);
	GLInstances->NumberOfInstances = 16;
}
