#ifndef R_SYSTEM_HEADER
#define R_SYSTEM_HEADER
//! Just something experimental
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <math.h>
#include <time.h>
#ifdef REX_DYNAMIC
#include <dlfcn.h>
#include <execinfo.h>
#endif
//! System specific description of file descriptor and stuff
#ifndef _WIN32 // Assume POSIX
typedef FILE* File;
typedef void* Pointer;
typedef size_t Size;
typedef unsigned int Uint;
typedef pid_t ProcessID;
#else
// WIN32 only supported
typedef HFILE* File;
typedef VOID* Pointer;
typedef SIZE_T Size;
typedef UINT Uint;
// Assume signed int
typedef INT ProcessID;
#endif
typedef struct {
	//! Write formatted output to console
	void  (*WriteLine)(const char*, ...);
	//! Read 'raw' input from console, with a specified length
	char* (*ReadLine)(char*, Uint);
	//! Console specific newline character, could be a LF, could be
	//! a CR+LF, could be anything
	unsigned char NewLine;
} REX_CONSOLE;
typedef struct {
	//! System specific IO access functions
	File (*Open)(char*, char*);
	int  (*Close)(File);
	Size (*Read)(Pointer, Size, Size, File);
	Size (*Write)(Pointer, Size, Size, File);
	//! Write in printf format
	int  (*WriteLine)(File, char*);
} REX_IO;
typedef struct {
	char* SystemName;
	char* SystemVersion;
	void (*GetApplicationPath)(char*, Uint);
	ProcessID (*GetProcessID)(void);
} REX_SYSINFO;
typedef struct {
	REX_CONSOLE Console;
	REX_IO IO;
	REX_SYSINFO SystemInfo;
} REX_SYSTEM_STRUCT;
//! Declare global struct
extern REX_SYSTEM_STRUCT System;
#endif
