#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <r3x_cpu.h>
#include <r3x_stack.h>
#include <r3x_format.h>
#include <r3x_exception.h>
#include <r3x_stream.h>
#ifdef REX_GRAPHICS
#include <r3x_graphics.h>
#endif
#include <r3x_dispatcher.h>
#include <r3x_bios.h>
#include <r3x_version.h>
#include <nt_malloc.h>
#include <r3x_dynamic.h>
#include <assert.h>
char* ApplicationPath = NULL;
char* ExecutableName = NULL;
void GetApplicationPath(void);
r3x_cpu_t* r3_cpu = NULL;
r3x_header_t* r3_header = NULL;
void quitSDL(void);
int main(int argc, char* argv[])
{
	GetApplicationPath();
	init_stack_construct();
	#ifdef REX_GRAPHICS
	// Set this to true to enable debugging info.
	nt_malloc_init(false);
	#else
	nt_malloc_init(false);
	#endif
	load_lib_manager();
	double freqcpu = 0;
	init_stream_manager();
	if (argc < 2){
		printf("main: Expected Argument : executable name!\n");
	}
	for(int i = 0; i < argc; i++){
		if(strncmp(argv[i], "-f", 2)==0){
			if(i+1 >= argc){
				printf("Error: -f option. Frequency not specified\n");
				exit(EXIT_FAILURE);
			}
			freqcpu = atof(argv[i+1]);
		} else if(strncmp(argv[i], "-exe", 4)==0){
			if(i+1 >= argc){
				printf("Error: -exe option. filename not specified\n");
				exit(EXIT_FAILURE);
			}
			ExecutableName = argv[i+1];
		}
	}
	atexit(quitSDL);
	// Install signal handlers
	signal(SIGSEGV, REX_EXCEPTION_HANDLER);
	signal(SIGINT,  REX_EXCEPTION_HANDLER);
	// Print version
	printf("%s\n", R3X_SYSTEM_VERSION);
	// Allocate memory for CPU structure
	r3_cpu = nt_malloc(sizeof(r3x_cpu_t));
	r3_cpu->Graphics = NULL;
	// Allocate memory for BIOS's header
	r3x_header_t* bios_header = nt_malloc(sizeof(r3x_header_t));
	// Initialise Memory
	r3_cpu->Memory = r3x_load_executable(ExecutableName, r3_header);
	#ifdef REX_GRAPHICS
	// Initialise Graphics Device and load default font.
	r3_cpu->Graphics = InitGraphics();
	r3_cpu->Graphics->font = loadfont("./bios/128x128.png");
	#endif
	// Run the BIOS
	r3_cpu->MemorySize = 512;
	r3_cpu->RootDomain = r3x_init_domain();
	r3x_load_bios(bios_header, r3_cpu);
	r3x_cpu_loop(r3_cpu, bios_header);
	// Now run the program
	r3_cpu->use_frequency = true;
	r3_cpu->CPUFrequency = freqcpu;
	if(freqcpu==0.0f){
		r3_cpu->CPUFrequency = 0;
		r3_cpu->use_frequency = false;
	}
	r3_header = (r3x_header_t*)&r3_cpu->Memory[PROG_EXEC_POINT];
	r3_cpu->InstructionPointer = r3_header->r3x_init;
	r3_cpu->MemorySize = r3_header->total_size + PROG_EXEC_POINT;
	r3_cpu->HeapAddr = r3_cpu->MemorySize;
	r3x_cpu_loop(r3_cpu, r3_header);
	// Show exit status
	printf("Program exitted with status: %u\n", Stack.GetItem(r3_cpu->Stack, r3_cpu->Stack->top_of_stack-1));
	// Free all 
	nt_freeall();
	// Exit
	exit(0);
}
void quitSDL(void) { 
	#ifdef REX_GRAPHICS
	SDL_Quit();
	#else
	printf("\n");
	#endif
}
void GetApplicationPath(void) { 
	#ifdef __linux__
	char buf[FILENAME_MAX];
	memset(buf, 0, sizeof(buf));
	/* Note we use sizeof(buf)-1 since we may need an extra char for NUL. */
	if (readlink("/proc/self/exe", buf, sizeof(buf)-1) < 0)
	{
		/* There was an error...  Perhaps the path does not exist
		* or the buffer is not big enough.  errno has the details. */
		perror("readlink");
		printf("FATAL: Unable to read /proc/self/exe!");
		exit(EXIT_FAILURE);
	}
	ApplicationPath = malloc(strlen(buf)+1);
	strcpy(ApplicationPath, buf);
	for(int i = strlen(ApplicationPath); i != 0; i--){
		// Backwards babe...
		/** Remove the executable name from the path **/
		if(ApplicationPath[i] == '/'){
			memset((char*)((intptr_t)ApplicationPath + i), 0, strlen(ApplicationPath)-i);
			return;
		}
	}
	#else 
	#error "GetApplicationPath unimplemented for target, please write your own fucking implementation"
	#endif
}
void PrintHelp(void) {
	
}
