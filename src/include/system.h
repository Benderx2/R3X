#ifndef R_SYSTEM_HEADER
#define R_SYSTEM_HEADER
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
#endif
