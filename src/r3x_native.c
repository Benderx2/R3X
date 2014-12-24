#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* For dlopen(), dlsym(), and dlerror()s */
#include <r3x_native.h>
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
	
	native_handle_t* newhandle = (native_handle_t*)malloc(sizeof(native_handle_t));
	newhandle->soname = name;
	newhandle->handle = handle;
	newhandle->next = NULL;
	void (*Start)(r3x_cpu_t*);
	*(void**)(&Start) = dlsym(handle, "Start");
	printf("Finding Start....\n");
	if(Start == NULL)
	{
		printf("WARNING: Start function not found!\n");
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
		printf("WARNING: Requested function not found! Name: %s\n", name);
	}
	return (*function)();
}