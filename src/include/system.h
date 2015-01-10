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
#include <stdarg.h>
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
typedef bool Boolean;
//! System specific description of file descriptor and stuff
#ifndef _WIN32 // Assume POSIX
typedef FILE* File;
typedef void* Pointer;
typedef const void* CPointer;
typedef size_t Size;
typedef unsigned int Uint;
typedef pid_t ProcessID;
typedef int Exception;
#else
// WIN32 only supported
typedef HFILE* File;
typedef VOID* Pointer;
typedef SIZE_T Size;
typedef UINT Uint;
typedef INT ProcessID;
typedef INT Exception;
#endif
typedef struct {
	//! Write formatted output to console
	int  (*WriteLine)(const char*, ...);
	//! Read 'raw' input from console, with a specified length
	char* (*ReadLine)(char*, Uint);
	//! Console specific newline character, could be a LF, could be
	//! a CR+LF, could be anything
	unsigned char NewLine;
} REX_CONSOLE;
typedef struct {
	//! System specific IO access functions
	File (*Open)(const char*, const char*);
	int  (*Close)(File);
	Size (*Read)(Pointer, Size, Size, File);
	Size (*Write)(CPointer, Size, Size, File);
	//! Write in printf format
	int  (*WriteLine)(File, const char*, ...);
} REX_IO;
typedef struct {
	Exception InvalidMemoryAccessException;
	Exception FloatingPointException;
	Exception ProgramInterruption;
	Exception ProgramExitException;
	Exception ProgramAlaramException;
	Exception UserException;
	void (*RegisterException)(int, void(*)(int));
} REX_EXCEPTIONS;
typedef struct {
	char* SystemName;
	char* SystemVersion;
	char* EnviromentVariables;
	void (*GetApplicationPath)(char*, Uint);
	ProcessID (*GetProcessID)(void);
} REX_SYSINFO;
typedef struct {
	REX_CONSOLE Console;
	REX_IO IO;
	REX_EXCEPTIONS Exceptions;
	REX_SYSINFO SystemInfo;
	Pointer (*AllocateMemory)(Size);
	Pointer (*FreeMemory)(Size);
} REX_SYSTEM_STRUCT;
//! Declare global struct
extern REX_SYSTEM_STRUCT System;
void InitializeREXSubsystem(Boolean DebugFlags);
#endif
