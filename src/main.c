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
#include <big_endian.h>
#include <assert.h>
char* ApplicationPath = NULL;
char* ExecutableName = NULL;
void GetApplicationPath(void);
double freqcpu = 0;
r3x_cpu_t* r3_cpu = NULL;
r3x_header_t* r3_header = NULL;
void quitSDL(void);
void ParseArguments(int argc, char* argv[]);
void PrintHelp(void);
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
	init_stream_manager();
	if (argc < 2){
		printf("main: Expected Argument : executable name!\n");
		exit(EXIT_FAILURE);
	}
	ParseArguments(argc, argv);
	atexit(quitSDL);
	// Install signal handlers
	signal(SIGSEGV, REX_EXCEPTION_HANDLER);
	signal(SIGINT,  REX_EXCEPTION_HANDLER);
	// Print version
	printf("%s\n", R3X_SYSTEM_VERSION);
	// Allocate memory for CPU structure
	r3_cpu = nt_malloc(sizeof(r3x_cpu_t));
	r3_cpu->Graphics = NULL;
	// Initialise Memory
	r3_cpu->Memory = r3x_load_executable(ExecutableName, r3_header);
	#ifdef REX_GRAPHICS
	// Initialise Graphics Device and load default font.
	r3_cpu->Graphics = InitGraphics();
	r3_cpu->Graphics->font = loadfont("./bios/128x128.png");
	#endif
	// Run the BIOS
	r3_cpu->RootDomain = r3x_init_domain();
	r3_header = (r3x_header_t*)&r3_cpu->Memory[PROG_EXEC_POINT];
	#ifdef R3X_BIG_ENDIAN
	r3_cpu->MemorySize = BIG_ENDIAN_INT(r3_header->total_size) + PROG_EXEC_POINT;
	r3_header->r3x_init = BIG_ENDIAN_INT(r3_header->r3x_init);
	#else
	r3_cpu->MemorySize = r3_header->total_size + PROG_EXEC_POINT;
	#endif
	r3_cpu->HeapAddr = r3_cpu->MemorySize;
	printf("init: %u\n", r3_header->r3x_init);
	r3_cpu->use_frequency = true;
	r3_cpu->CPUFrequency = freqcpu;
	if(freqcpu==0.0f){
		r3_cpu->CPUFrequency = 0;
		r3_cpu->use_frequency = false;
	}
	r3x_load_bios(r3_cpu);
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
void ParseArguments(int argc, char* argv[]){
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
		} else if(strncmp(argv[i], "-h", 2)==0||strncmp(argv[i], "-help", 5)==0||strncmp(argv[i], "--help", 6)==0){
			PrintHelp();
			exit(EXIT_SUCCESS);
		}
	}
}
void PrintHelp(void) {
	printf("%s\n", R3X_SYSTEM_VERSION);
	printf("\n\n");
	printf("R3X Virtual Machine - help\n");
	printf("Usage r3x_vm.out -exe [exe file name] optional(-f [CPU Frequency])\n\n");
	printf("Copyright (C) 2015 Benderx2 <https://github.com/Benderx2>\n\n");
	printf("R3X is free software and is licensed under 2-clause BSD License <http://opensource.org/licenses/BSD-2-Clause>\n");
	printf("You are free to modify, redistrubute, or make profit of this program provided\n");
	printf("you respect it's license and terms.\n");
	printf("\n\n");
	printf("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n");
	printf("AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,\n"); 
	printf("THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n"); 
	printf("IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\n"); 
	printf("INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n"); 
	printf("BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;\n"); 
	printf("OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, \n");
	printf("WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) \n");
	printf("ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
}
