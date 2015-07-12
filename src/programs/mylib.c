#include "rex/virtual-machine.h"
r3x_cpu_t* MyCPU;
void Start(r3x_cpu_t* CPU) {
	MyCPU = CPU;
	return;
}
uint32_t myfunc(void) {
	printf("getting argument 1: %u\n", GetArgument(MyCPU, 1,1));
	return 0;
}
