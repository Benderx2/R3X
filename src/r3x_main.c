#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
r3x_cpu_t* r3_cpu = NULL;
r3x_header_t* r3_header = NULL;
void quitSDL(void);
int main(int argc, char* argv[])
{
	init_stack_construct();
	#ifdef REX_GRAPHICS
	nt_malloc_init(true);
	#else
	nt_malloc_init(false);
	#endif
	load_lib_manager();
	init_stream_manager();
	if (argc < 2){
		printf("main: Expected Argument : executable name!\n");
	}
	atexit(quitSDL);
	signal(SIGSEGV, REX_EXCEPTION_HANDLER);
	signal(SIGINT,  REX_EXCEPTION_HANDLER);
	printf("%s\n", R3X_SYSTEM_VERSION);
	r3_cpu = nt_malloc(sizeof(r3x_cpu_t));
	r3_cpu->Graphics = NULL;
	r3x_header_t* bios_header = nt_malloc(sizeof(r3x_header_t));
	r3_cpu->Memory = r3x_load_executable(argv[1], r3_header);
	#ifdef REX_GRAPHICS
	r3_cpu->Graphics = InitGraphics();
	r3_cpu->Graphics->font = loadfont("./bios/128x128.png");
	#endif
	r3_cpu->MemorySize = 512;
	r3_cpu->RootDomain = r3x_init_domain();
	r3x_load_bios(bios_header, r3_cpu);
	r3x_cpu_loop(r3_cpu, bios_header);
	r3_header = (r3x_header_t*)&r3_cpu->Memory[PROG_EXEC_POINT];
	r3_cpu->InstructionPointer = r3_header->r3x_init;
	r3_cpu->MemorySize = r3_header->total_size + PROG_EXEC_POINT;
	r3_cpu->HeapAddr = r3_cpu->MemorySize;
	r3x_cpu_loop(r3_cpu, r3_header);
	printf("Program exitted with status: %u\n", Stack.GetItem(r3_cpu->Stack, r3_cpu->Stack->top_of_stack-1));
	nt_freeall();
	exit(0);
}
void quitSDL(void) { 
	#ifdef REX_GRAPHICS
	SDL_Quit();
	#else
	printf("\n");
	#endif
}
