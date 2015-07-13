#include "rex/virtual-machine.h"
r3x_cpu_t* MyCPU;
void Start(r3x_cpu_t* CPU) {
	MyCPU = CPU;
	return;
}
uint32_t myfunc(void) {
	printf("getting argument 1 (STRING): %s\n", (char*)GetLinearAddress(MyCPU, GetArgument(MyCPU, 1,1)));
	printf("returning 666\n");
	return 666;
}
