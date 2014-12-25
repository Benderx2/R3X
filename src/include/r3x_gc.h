#ifndef __R3X_GC_H
#define __R3X_GC_H
#include <r3x_cpu.h>
void gc_init(void);
void gc_notify(r3x_cpu_t* CPU);
void gc_mark(r3x_cpu_t* CPU);
void gc_clean(r3x_cpu_t* CPU);
#endif
