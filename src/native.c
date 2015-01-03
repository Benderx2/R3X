/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* For dlopen(), dlsym(), and dlerror()s */
#include <r3x_native.h>
#include <nt_malloc.h>
native_handle_t* head;
native_handle_t* tail;
int number_of_handles;
void* load_native_library(char* name, r3x_cpu_t* CPU)
{
	void *handle;
	handle = dlopen(name, RTLD_LAZY);
	if(!handle){	
		printf("\nFATAL: Unable to load shared object: %s\n", name);
	}	
	
	native_handle_t* newhandle = nt_malloc(sizeof(native_handle_t));
	newhandle->soname = name;
	newhandle->handle = handle;
	newhandle->next = NULL;
	void (*Start)(r3x_cpu_t*);
	*(void**)(&Start) = dlsym(handle, "Start");
	printf("Finding Start....\n");
	if(Start == NULL)
	{
		printf("ERROR: Start function not found!\n");
		exit(EXIT_FAILURE);
	}
	else {
		(*Start)(CPU);
	}
	if(number_of_handles == 0)
	{
		head = newhandle;
		tail = newhandle;
	}
	else {		
		tail->next = newhandle;
		tail = newhandle;
	}
	number_of_handles++;
	return handle;
}
void* returnhandle(char* soname)
{
	native_handle_t* current_handle = head;
	for(int i = 0; i <= number_of_handles; i++)
	{	
		if(!strcmp(current_handle->soname, soname)){
			return current_handle->handle;
		}
		else {
			if(current_handle->next == NULL)
			{
				return NULL;
			}
			current_handle = (struct native_handle*)current_handle->next;
		}
	}
	return NULL;
}
int native_call(char* name, void* handle)
{
	// Clear existing errors.
	dlerror();
	int (*function)(void);
	*(void**)(&function) = dlsym(handle, name);
	if(function == NULL)
	{
		printf("ERROR: Requested function not found! Name: %s\n", name);
		exit(EXIT_FAILURE);
	}
	return (*function)();
}