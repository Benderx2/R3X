#ifndef __R3X_GARBAGE_COLLECTOR_H
#define __R3X_GARBAGE_COLLECTOR_H
#include <system.h>
#include <r3x_cpu.h>
typedef enum {
	RX_GC_ALIVE,
	RX_GC_DEAD,
} RX_GC_STATUS;
typedef struct {
	uint32_t** Addresses;
	unsigned int TotalReferences;
} GC_VARIABLE_REFERENCE;
typedef struct {
	r3x_cpu_t* CPU;
	GC_VARIABLE_REFERENCE* Variables;
	unsigned int TotalObjects;
} GC_VARIABLE_LIST;

GC_VARIABLE_LIST* InitializeGC(r3x_cpu_t* CPU);

unsigned int GCCreateVariableInstance(GC_VARIABLE_LIST* List, uint32_t Address, uint32_t Value);

void GCAddReference(GC_VARIABLE_LIST* List, unsigned int ObjectID, uint32_t Address);

void GCMark(GC_VARIABLE_LIST*);

void GCCollect(GC_VARIABLE_LIST*);
#endif
