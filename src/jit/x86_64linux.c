#include "jit.h"
#include "x86_64.h"
#include <nt_nalloc.h>
RX_CPU_X64* CPUPointer;
RX_CPU_X64* Initializex64CoreModule(void){
	CPUPointer = nt_malloc(sizeof(RX_CPU_X64));
	CPUPointer->x64_Handlers = nt_malloc(sizeof(RX_TARGET_CPU));
	x64OS = nt_malloc(sizeof(RX_TARGET_OS));
	/**
	 * Do codegen initialization
	**/
	return CPUPointer;
}
void Intializex64CoreSubModule(void){
	return;
}
