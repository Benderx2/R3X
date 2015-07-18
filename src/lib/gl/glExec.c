#define REX_GRAPHICS
#define GL_TRUE 0
#define GL_FALSE 1
#define SO_NAME "glExec.so"

#include <virtual-machine.h>
#include <nt_malloc.h>

/* gl implementation for r3x */

r3x_cpu_t* CPU = NULL;

typedef struct GL_EXECUTION_INSTANCE {
	unsigned char* Assembly;
	unsigned int Available;
	int32_t(*GLCompiledFunction)(void);
} GLExecutionInstance;

typedef struct {
	unsigned int NumberOfInstances;
	GLExecutionInstance* Instances;
} GLExecutionInstances;

GLExecutionInstances* GLInstances;
GLExecutionInstance* CurrentGLInstanceID;

void Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
	nt_malloc_init(false);
	GLInstances = nt_malloc(sizeof(GLExecutionInstances));
	GLInstances->Instances = nt_malloc(sizeof(GLExecutionInstance)*16);
	GLInstances->NumberOfInstances = 16;
}

uint32_t gl_CreateInstance(void) {
	for(unsigned int i = 0; i < GLInstances->NumberOfInstances; i++) {
		if(GLInstances->Instances[i].Available == GL_TRUE) {
			GLInstances->Instances[i].Available = GL_FALSE;
			return (uint32_t)i;
		}
	}
	
	GLInstances->Instances = nt_realloc((void*)GLInstances->Instances, sizeof(GLExecutionInstance)*GLInstances->NumberOfInstances+16);
	GLInstances->NumberOfInstances += 16;
	
	GLInstances->Instances[GLInstances->NumberOfInstances].Available = GL_FALSE;
	return (uint32_t)GLInstances->NumberOfInstances;
}
uint32_t gl_EnterInstance(void) {
	if(CurrentGLInstanceID != NULL) {
		printf("[%s]: Trying to enter another instance without exitting the previous one!\n", SO_NAME);
		return (uint32_t)-1;
	}
	uint32_t Local_CurrentGLInstanceID = GetArgument(CPU, 1,1);
	if(Local_CurrentGLInstanceID >= GLInstances->NumberOfInstances) {
		printf("[%s]: Invalid GLInstance ID.\n", SO_NAME);
		return (uint32_t)-1;
	} else if(GLInstances->Instances[Local_CurrentGLInstanceID].Available == GL_FALSE) {
		printf("[%s]: Trying to use an uninitialised GLInstance ID.\n", SO_NAME);
		return (uint32_t)-1;
	}
	CurrentGLInstanceID = &(GLInstances->Instances[Local_CurrentGLInstanceID]);
	return 0;
}
uint32_t gl_ExitInstance(void) {
	if(CurrentGLInstanceID == NULL) {
		printf("[%s]: gl_ExitInstance called without corresponding gl_EnterInstance.\n", SO_NAME);
		return (uint32_t)-1;
	}
	CurrentGLInstanceID = NULL;
	return 0;
}
uint32_t gl_FreeAll(void) {
	nt_freeall();
	return 0;
}
