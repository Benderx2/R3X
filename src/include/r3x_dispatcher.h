#ifndef __R3X_DISPATCHER_H
#define __R3X_DISPATCHER_H
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <r3x_cpu.h>
#include <r3x_stack.h>
r3x_global_domain_t* r3x_init_domain(void);
unsigned int r3x_dispatch_job(int InstructionPointer, int CycleUpdate, r3x_global_domain_t* Domain, bool ismain);
int r3x_load_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain);
void r3x_save_job_state(r3x_cpu_t* CPU, r3x_global_domain_t* Domain);
void r3x_exit_job(r3x_global_domain_t* Domain);
#endif
