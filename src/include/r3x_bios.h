#ifndef __R3X_BIOS_H
#define __R3X_BIOS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <r3x_cpu.h>
#define REX_BIOS_SIZE 512
void r3x_load_bios(r3x_header_t* header, r3x_cpu_t* CPU);
#endif
