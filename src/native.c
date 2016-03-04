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
#ifdef REX_DYNAMIC
#include <r3x_native.h>
#include <nt_malloc.h>
extern char* ApplicationPath;
native_handle_t* head;
native_handle_t* tail;
int number_of_handles;
extern r3x_cpu_t* r3_cpu;
void* load_native_library(char* name, r3x_cpu_t* CPU)
{
	void *handle;
	if(!strcmp(name, "rstdlib")) {
		char* tempstr = nt_malloc(strlen(ApplicationPath)+strlen("/rstdlib.so")+1);
		strcpy(tempstr, ApplicationPath);
		strcat(tempstr, "/rstdlib.so");
		handle = dlopen(tempstr, RTLD_LAZY);
		nt_free(tempstr);
	} else {
		handle = dlopen(name, RTLD_LAZY);
	}
	if(!handle){
		// Try opening it from the VM directory.
		char* tempstr = nt_malloc(strlen(ApplicationPath)+strlen(name)+3);
		strcpy(tempstr, ApplicationPath);
		tempstr[strlen(tempstr)] = '/';
		tempstr[strlen(tempstr)+1] = 0;
		strcat(tempstr, name);
		printf("[Alert]Library %s not found in Application directory. Trying to load from %s\n", name, tempstr);
		handle = dlopen(tempstr, RTLD_LAZY);	
	}	
	if(!handle) {
		printf("\nFATAL: Unable to load shared object: %s\n", name);
		exit(0);
	}
	
	native_handle_t* newhandle = nt_malloc(sizeof(native_handle_t));
	newhandle->soname = strdup(name);
	newhandle->handle = handle;
	newhandle->next = NULL;
	void (*Start)(r3x_cpu_t*);
	*(void**)(&Start) = dlsym(handle, "Start");
	if(Start == NULL)
	{
		printf("ERROR: Start function not found in native library! Exitting..\n");
		exit(0);
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
		if(current_handle->soname != NULL) {
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
#endif
