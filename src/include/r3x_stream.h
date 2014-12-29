#ifndef __R3X_STREAM_H
#define __R3X_STREAM_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nt_malloc.h>
void init_stream_manager(void);
unsigned int stream_open(char* name);
void stream_close(unsigned int handle);
void stream_seek(unsigned int handle, long int off, int origin);
unsigned int stream_tell(unsigned int handle);
unsigned int stream_read(void* ptr, unsigned int handle, size_t count);
unsigned int stream_write(void* ptr, unsigned int handle, size_t count);
#endif
