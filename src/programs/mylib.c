#include "rex/virtual-machine.h"
r3x_cpu_t* MyCPU;
void Start(r3x_cpu_t* CPU) {
	MyCPU = CPU;
	return;
}
uint32_t myfunc(void) {
	printf("Calling from dynamic native library. Returning 123456789\n");
	Stack.Push(MyCPU->Stack, 0xFFFFFFFF); // Push random shit to CPU stack.
	return 123456789;
}
