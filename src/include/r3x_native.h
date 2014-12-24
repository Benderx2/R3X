#ifndef __R3X_NATIVE_H
#define __R3X_NATIVE_H
#include <stdint.h>
#include <r3x_cpu.h>
// Special crap for Windows.
#ifdef _WIN32
#include <_win32/dlfcn.h>
#else 
#include <dlfcn.h>
#endif
typedef struct native_handle {
	char* soname;
	void* handle;
	struct native_handle* next;
} native_handle_t;
void* load_native_library(char* name, r3x_cpu_t* CPU);
int native_call(char* name, void* handle);
void* returnhandle(char* soname);
#endif