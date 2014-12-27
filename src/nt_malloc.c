#include <nt_malloc.h>
void** malloc_pointers = NULL;
int malloc_pointer_sz = 0;
int number_of_allocs = 0;
int number_of_frees = 0;
int total_allocs = 0;
bool debugflag = false;
bool already_init = false;
bool isAllFreed = false;
size_t total_size_used = 0;
int nt_malloc_init(bool flag) { 
	if(already_init == false) { 
		malloc_pointers = malloc(16*sizeof(void**));
		malloc_pointer_sz = 15;
		for(int i = 0; i < malloc_pointer_sz; i++) { 
			malloc_pointers[i] = NULL;
		}
		debugflag = flag;
		#ifndef NT_MALLOC_ATEXIT_DISABLE
		atexit(nt_atexit);
		#endif
		if(debugflag == true) { 
			fprintf(stdout, "NT_MALLOC_INIT: Initialised POSIX malloc wrapper\n");
		}
		already_init = true;
		return 0;
	} else { 
			fprintf(stderr, "NT_MALLOC_INIT: Attempt to initialise POSIX wrapper twice\n");
			return -1;	
	}
	
}
void* nt_realloc(void* ptr, size_t size) {
	if(already_init == false) { 
		fprintf(stderr, "NT_REALLOC: Call 'nt_malloc_init(bool)' before using nt_malloc(uint). ABORTING..\n");
		nt_freeall();
		exit(EXIT_FAILURE);
	}
	total_size_used += size;
	if(ptr == NULL) { 
		return nt_malloc(size); // Return malloc'd buffer if ptr == NULL
	}
	// Is it allocated?
	for(int i = 0; i < malloc_pointer_sz; i++) { 
			if(malloc_pointers[i] == ptr) { 
				void* tmp = realloc(ptr, size);
				if(tmp==NULL) {
					fprintf(stderr, "NT_REALLOC: Realloc returned NULL. ABORING..");
					exit(0);
				}
				malloc_pointers[i] = tmp;
				if(debugflag == true) { 
					fprintf(stdout, "NT_REALLOC: Reallocating pointer: %p, to size: %u, to a new address: %p", ptr, (unsigned int)size, malloc_pointers[i]);
				}
				return malloc_pointers[i];			
			}
				
	}
	if(debugflag == true) { 
		fprintf(stderr, "NT_REALLOC: attempt realloc an already freed/unallocated memory address. Please ensure you're ONLY using nt_malloc to allocate memory.\n");
	}
	return NULL;
}
void* nt_calloc(unsigned int unit, size_t size) { 
	return nt_malloc(unit*size);
}
void* nt_malloc(size_t size) { 
	if(already_init == false) { 
		fprintf(stderr, "NT_MALLOC: Call 'nt_malloc_init(bool)' before using nt_malloc(uint). ABORTING..\n");
		nt_freeall();
		exit(EXIT_FAILURE);
	}
	total_size_used += size;
	// Allocate memory and check if it's NULL
	void* new_ptr = malloc(size);
	if(new_ptr == NULL) { 
		if(debugflag==true){
			fprintf(stderr, "NT_MALLOC: malloc returned NULL. ABORTING..\n");
		}
		exit(1);
	}
	if(debugflag==true) { 
		fprintf(stdout, "NT_MALLOC: allocated %u bytes of memory for allocation no. %u at address %p\n", (unsigned int)size, total_allocs, new_ptr); 
	}
	isAllFreed = false;
	// Clear memory range...
	memset(new_ptr, 0, size);
	// Now check if we are out of space in our pointer buffer
	if(number_of_allocs >= malloc_pointer_sz) {
		void** new_buf = malloc((malloc_pointer_sz + 16)*sizeof(void**));
		for(int i = 0; i < malloc_pointer_sz + 16; i++) { 
			new_buf[i] = NULL;		
		}
		for(int i = 0; i < malloc_pointer_sz; i++) {
			new_buf[i] = malloc_pointers[i];
		}
		free(malloc_pointers);
		malloc_pointers = new_buf;
		malloc_pointer_sz += 16;
	}
	// Find a NULL pointer in array and assign it.
	for(int i = 0; i < malloc_pointer_sz; i++) { 
		if(malloc_pointers[i] == NULL)  {
			malloc_pointers[i] = new_ptr;
			number_of_allocs++;
			total_allocs++;
			return new_ptr;		
		}
	}
	if(debugflag==true) { 
		fprintf(stderr, "NT_MALLOC: internal fault. Can't find an empty pointer in array. Please report this bug! ABORTING..\n");
	}
	nt_freeall();
	exit(EXIT_FAILURE);
}

int nt_free(void* ptr) { 
	// NOTE: Pointer must be allocated using nt_malloc
	// Find the pointer
	for(int i = 0; i < malloc_pointer_sz; i++) { 
		if((intptr_t)malloc_pointers[i] == (intptr_t)ptr) { 
			malloc_pointers[i] = NULL;
			free(ptr);
			number_of_allocs--;
			number_of_frees++;
			if(debugflag==true) {
				fprintf(stdout, "NT_FREE: freed memory address %p\n", ptr);
			}
			return 0;		
		}	
	}	
	if(debugflag==true) {
	fprintf(stderr, "NT_FREE: attempt to free an already freed/unallocated memory address. Please ensure you're ONLY using nt_malloc to allocate memory.\n");
	}
	return -1;
}
void nt_freeall(void) { 
	for(int i = 0; i < malloc_pointer_sz; i++) { 
		if(malloc_pointers[i] != NULL) { 
			free(malloc_pointers[i]); 
			if(debugflag==true) { 
				printf("NT_FREEALL: freed address %p\n", malloc_pointers[i]);
			}
			malloc_pointers[i] = NULL;
			number_of_frees++;
		}	
	}
	free(malloc_pointers);
	isAllFreed = true;
}
void nt_atexit(void) {
	if(debugflag == true) { 
		fprintf(stdout, "NT_ATEXIT: Total Number of Allocations: %d\n", total_allocs);
		fprintf(stdout, "NT_ATEXIT: Total Number of Frees: %d\n", number_of_frees);
		fprintf(stdout, "Don't worry we'll free it for ya!\n");
		fprintf(stdout, "Total Memory used by application (using nt_malloc, nt_realloc, nt_calloc) %zu bytes\n", total_size_used);
	}
	if(isAllFreed != true) { 
		nt_freeall();
	} 
}

