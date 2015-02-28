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
#include <r3x_cpu.h>
#include <r3x_stack.h>
#include <r3x_format.h>
#include <r3x_exception.h>
#include <r3x_stream.h>
#include <r3x_dispatcher.h>
#include <r3x_bios.h>
#include <r3x_version.h>
#include <nt_malloc.h>
#include <r3x_dynamic.h>
#include <big_endian.h>
#ifdef REX_GRAPHICS
#include <r3x_graphics.h>
#endif
extern uint32_t text_begin, text_size, data_begin, data_size;
void quitSDL(void);
void ParseArguments(int argc, char* argv[]);
void PrintHelp(void);
void GetApplicationPath(void);
// graphics and stuff
unsigned int ScreenWidth = 640;
unsigned int ScreenHeight = 480;
unsigned int FontHeight = 8;
unsigned int FontWidth = 8;
double FontScale = 0.5f;
char* FontFileName = NULL;
char* ProgramArguments = NULL;
// names for vars
double ChosenCPUFrequency = 0;
unsigned int max_stack = DEFAULT_MAX_STACK_SIZE;
r3x_cpu_t* r3_cpu = NULL;
r3x_header_t* r3_header = NULL;
char* ApplicationPath = NULL;
char* ExecutableName = NULL;
char* DefaultFontFileName = "/bios/128x128.png";
bool UseServer = false;
int main(int argc, char* argv[])
{
	GetApplicationPath();
	init_stack_construct();
	#ifdef R3X_DEBUG
	nt_malloc_init(true);
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
	if(UseServer == true){
		printf("VM is running under client mode\n");
	}
	atexit(quitSDL);
	// Install signal handlers
	signal(SIGSEGV, REX_EXCEPTION_HANDLER);
	signal(SIGINT,  REX_EXCEPTION_HANDLER);
	#ifndef _WIN32
	signal(SIGUSR1, SIGUSR1_handler);
	#endif
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
	char* default_fontname = malloc(strlen(ApplicationPath) + strlen(DefaultFontFileName)+1);
	strcpy(default_fontname, ApplicationPath);
	strcat(default_fontname, DefaultFontFileName);
	r3_cpu->Graphics->font = loadfont(default_fontname);
	#endif
	// Run the BIOS
	r3_cpu->RootDomain = r3x_init_domain();
	r3_header = (r3x_header_t*)&r3_cpu->Memory[PROG_EXEC_POINT];
	#ifdef R3X_BIG_ENDIAN
	r3_cpu->MemorySize = BIG_ENDIAN_INT(r3_header->total_size) + PROG_EXEC_POINT;
	r3_header->r3x_init = BIG_ENDIAN_INT(r3_header->r3x_init);
	#else
	r3_cpu->MemorySize = ((r3_header->total_size + PROG_EXEC_POINT)&0xFFFFF000)+SEGMENT_SIZE;
	#endif
	r3_cpu->CPUMemoryBlocks = BuildMemoryBlock(r3_cpu->MemorySize);
	r3_cpu->HeapAddr = r3_cpu->MemorySize;
	r3_cpu->use_frequency = true;
	r3_cpu->CPUFrequency = ChosenCPUFrequency;
	if(ChosenCPUFrequency==0.0f){
		r3_cpu->CPUFrequency = 0;
		r3_cpu->use_frequency = false;
	}
	r3x_load_bios(r3_cpu);
	//! Map the executable regions
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_EXEC, 0, SEGMENT_SIZE);
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_RW, 4096, SEGMENT_SIZE);
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_EXEC, text_begin, text_begin + text_size);
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_RW, data_begin, data_begin + data_size);
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_RW, data_begin + data_size, data_begin+data_size+SEGMENT_SIZE);
	MemoryMap(r3_cpu->CPUMemoryBlocks, RX_RONLY, PROG_EXEC_POINT, PROG_EXEC_POINT + SEGMENT_SIZE);
	load_dependencies(r3_cpu);
	r3_header = (r3x_header_t*)&(r3_cpu->Memory[PROG_EXEC_POINT]);
	r3x_cpu_loop(r3_cpu, r3_header, ProgramArguments);
	free(ApplicationPath); // Allocated using strdup
	// Free all
	nt_freeall();
	// Exit
	return 0;
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
	#ifndef FILENAME_MAX
	#warning "Assuming max filename as 1024 bytes"
	char buf[1024];
	#else
	char buf[FILENAME_MAX];
	#endif
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
	ApplicationPath = strdup(buf);
	for(size_t i = strlen(ApplicationPath); i != 0; i--){
		// Backwards babe...
		/** Remove the executable name from the path **/
		if(ApplicationPath[i] == '/'){
			memset((char*)((intptr_t)ApplicationPath + i), 0, strlen(ApplicationPath)-i);
			return;
		}
	}
	#elif defined WIN32
	char buf[1024];
	memset(buf, 0, 1024);
	GetModuleFileName(NULL, buf, 1024);
	ApplicationPath = strdup(buf);
	for(size_t i = strlen(ApplicationPath); i != 0; i--){
		// Backwards babe...
		/** Remove the executable name from the path **/
		if(ApplicationPath[i] == '/' || ApplicationPath[i] == '\\' /*fkn win32 with fkn backslahes xxx*/){
			memset((char*)((intptr_t)ApplicationPath + i), 0, strlen(ApplicationPath)-i);
			return;
		}
	}
	printf("$TESTING: Application Path: %s$\n", ApplicationPath);
	#else
	#warning "GetApplicationPath unimplemented for target, using RX_DIR_ENV. Be sure to export RX_DIR_ENV as the executable path of your directory!"
	char* env_read = getenv("RX_DIR_ENV");
	ApplicationPath = env_read;
	#endif
}
void ParseArguments(int argc, char* argv[]){
	for(int i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-f")){
			if(i+1 >= argc){
				printf("Error: -f option. Frequency not specified\n");
				exit(EXIT_FAILURE);
			}
			ChosenCPUFrequency = atof(argv[i+1]);
		} else if(!strcmp(argv[i], "-exe")){
			if(i+1 >= argc){
				printf("Error: -exe option. filename not specified\n");
				exit(EXIT_FAILURE);
			}
			ExecutableName = argv[i+1];
			printf("Executable Name %s\n", ExecutableName);
		} else if(!strcmp(argv[i], "-stack")){
			if(i+1 >= argc){
				printf("Error: -stack option, stack not specified.\n");
				exit(EXIT_SUCCESS);
			}
			max_stack = atoi(argv[i+1]);
		} else if(!strcmp(argv[i], "-w")){
			if(i+1 >= argc){
				printf("Error: -w option, width not specified.\n");
				exit(EXIT_SUCCESS);
			}
			ScreenWidth = atoi(argv[i+1]);
		} else if(!strcmp(argv[i], "-h")){
			if(i+1 >= argc){
				printf("Error: -h option, height not specified.\n");
				exit(EXIT_SUCCESS);
			}
			ScreenHeight = atoi(argv[i+1]);
		} else if(!strcmp(argv[i], "-fo")) {
			if(i + 1 >= argc) {
				printf("Error: -f option, font file name not specified. [ONLY PNG FORMATS SUPPORTED]\n");
				exit(EXIT_SUCCESS);
			}
			FontFileName = (char*)argv[i+1];
		} else if(!strcmp(argv[i], "-fw")) {
			if(i + 1 >= argc) {
				printf("Error: -fw option, font width not specified.\n");
				exit(EXIT_SUCCESS);
			}
			FontWidth = atoi(argv[i+1]);
		} else if(!strcmp(argv[i], "-fh")) {
			if(i + 1 >= argc) {
				printf("Error: -fh option, font height not specified.\n");
				exit(EXIT_SUCCESS);
			}
			FontHeight = atoi(argv[i+1]);
		} else if(!strcmp(argv[i], "-fs")) {
			if(i + 1 >= argc) {
				printf("Error: -fs option, font height not specified.\n");
				exit(EXIT_SUCCESS);
			}
			FontScale = atof(argv[i+1]);
		} else if(!strcmp(argv[i], "-s")){
			UseServer = true;
		} else if(!strcmp(argv[i], "-args")) {
			if(i+1 >= argc) {
				printf("Error: -args option, arguments not specified\n");
				exit(EXIT_FAILURE);
			}
			ProgramArguments = argv[i+1];
		} else if(strncmp(argv[i], "-help", 5)==0||strncmp(argv[i], "--help", 6)==0){
			PrintHelp();
			exit(EXIT_SUCCESS);
		}
	}
	if(ProgramArguments == NULL) {
		ProgramArguments = "NOARGS";
	}
}
void PrintHelp(void) {
	printf("%s\n", R3X_SYSTEM_VERSION);
	printf("\n\n"
		"R3X Virtual Machine - help\n"
		"Usage rxvm -exe [exe file name] optional(-f [CPU Frequency])\n\n"
		"Copyright (C) 2015 Benderx2 <https://github.com/Benderx2>\n\n"
		"R3X is free software and is licensed under 2-clause BSD License <http://opensource.org/licenses/BSD-2-Clause>\n"
		"You are free to modify, redistrubute, or make profit of this program provided\n"
		"you respect it's license and terms.\n"
		"\n\n"
		"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"
		"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,\n"
		"THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n"
		"IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\n"
		"INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n"
		"BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;\n"
		"OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, \n"
		"WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) \n"
		"ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
}
