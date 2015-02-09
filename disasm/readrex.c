#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef struct r3x_header {
	uint32_t header_id;
	uint32_t program_header;
	uint16_t minor;
	uint16_t major;
	uint32_t r3x_init;
	uint32_t r3x_text_size;
	uint32_t r3x_data;
	uint32_t r3x_data_size;
	uint32_t r3x_symbols;
	uint32_t r3x_symbolsize;
	uint32_t r3x_bss;
	uint32_t r3x_bss_size;
	uint32_t stack_size;
	uint32_t total_size;
	uint32_t nameaddr;
	uint32_t pulibsheraddr;
} r3x_header_t;
typedef struct r3x_symbol {
	uint32_t SymbolName;
	uint32_t SymbolStart;
	uint32_t SymbolEnd;
} r3x_symbol_t;
typedef struct REX_DYNAMIC_HEADER {
	uint32_t header;
	uint32_t minor;
	uint32_t major;
	uint32_t text_section;
	uint32_t text_size;
	uint32_t export_section;
	uint32_t export_size;
	uint32_t data_section;
	uint32_t data_size;
	uint32_t bss_section;
	uint32_t bss_size;
	uint32_t nameaddr;
	uint32_t publisheraddr;
} r3x_dynamic_header_t;
typedef struct export_struct {
	uint32_t function_id;
	uint32_t instruction_pointer;
} export_struct;

#define EXEC_HEADER 0xBA5EBA11
#define PROG_EXEC_POINT 0x100000
#define DLL_HEADER 0xDEADBEEF

void ParseArguments(int argc, char* argv[]);
void read_symbol_table(r3x_header_t* header, uint8_t* Memory, uint32_t size, uint32_t IP);

void PrintHelp(void);
char* InputFile = NULL;
char* InputBuffer = NULL;

int main(int argc, char* argv[]){
	ParseArguments(argc, argv);
	if(InputFile == NULL) {
		fprintf(stderr, "Input file not specified\n");
		exit(EXIT_FAILURE);
	}
	FILE* fpin = fopen(InputFile, "r+b");
	if(fpin == NULL){
		perror("fopen failed: ");
		exit(EXIT_FAILURE);
	}
	// seek to end of file
	fseek(fpin, 0L, SEEK_END);
	// get file size
	unsigned int size = ftell(fpin);
	// allocate memory
	uint8_t* InputBuffer = malloc(size);
	// seek reset
	fseek(fpin, 0L, SEEK_SET);
	// read all bytes
	unsigned int sizeread = fread(InputBuffer, sizeof(uint8_t), size, fpin);
	if(sizeread != size){
		fprintf(stderr, "fread failed.\n");
		perror("fread: ");
		exit(EXIT_FAILURE);
	}
	if(*((uint32_t*)&InputBuffer[0]) == EXEC_HEADER) {
		printf("Type: Executable\n");
		r3x_header_t* myheader = (r3x_header_t*)&InputBuffer[0];
		printf("Program header Index: 0x%X\n", myheader->program_header);
		printf("Version (Major).(Minor): %u.%u\n", myheader->major, myheader->minor);
		printf("Location of .text: %u\n", myheader->r3x_init);
		printf("Size of .text: %u\n", myheader->r3x_text_size);
		printf("Location of .symbols: %u\n", myheader->r3x_symbols);
		printf("Size of .symbols: %u\n", myheader->r3x_symbolsize);
		printf("Location of data: %u\n", myheader->r3x_data);
		printf("Size of .data: %u\n", myheader->r3x_data_size);
		printf("Location of .bss: %u\n", myheader->r3x_bss);
		printf("Size of .bss: %u\n", myheader->r3x_bss_size);
		if(myheader->nameaddr - PROG_EXEC_POINT > size || myheader->pulibsheraddr - PROG_EXEC_POINT > size) {
			printf("Invalid publisher/executable information\n");
		} else {
			printf("Name: %s\n", (char*)&InputBuffer[myheader->nameaddr - PROG_EXEC_POINT]);
			printf("Publisher: %s\n", (char*)&InputBuffer[myheader->pulibsheraddr - PROG_EXEC_POINT]);
		}
		if(myheader->r3x_symbols - PROG_EXEC_POINT > size) {
			printf("Invalid symbol table\n");
		} else {
			read_symbol_table(myheader, InputBuffer, size, 0);
		}
	} else if(*((uint32_t*)&InputBuffer[0]) == DLL_HEADER) {
		printf("Type: Dynamic Shared Library\n");
		r3x_dynamic_header_t* myheader = (r3x_dynamic_header_t*)&InputBuffer[0];
		printf("Program header Index: 0x%X\n", 0);
		printf("Version (Major).(Minor): %u.%u\n", myheader->major, myheader->minor);
		printf("Location of .text: %u\n", myheader->text_section);
		printf("Size of .text: %u\n", myheader->text_size);
		printf("Location of data: %u\n", myheader->data_section);
		printf("Size of .data: %u\n", myheader->data_size);
		printf("Location of .bss: %u\n", myheader->bss_section);
		printf("Size of .bss: %u\n", myheader->bss_size);
		if(myheader->nameaddr > size || myheader->publisheraddr > size) {
			printf("Invalid publisher/executable information\n");
		} else {
			printf("Name: %s\n", (char*)&InputBuffer[myheader->nameaddr - 0]);
			printf("Publisher: %s\n", (char*)&InputBuffer[myheader->publisheraddr - 0]);
		}
		printf(".export section: \n"); 
		if(myheader->export_section > size) {
			printf("Bad Export section\n");
		} else {
			export_struct* myexports = (export_struct*)&InputBuffer[myheader->export_section];
			for(unsigned int i = 0; i < myheader->export_size / sizeof(export_struct); i++){
				if(myexports[i].function_id > size) {
					printf("bad export\n");
				} else {
					printf("Export %s, to function at: %u\n", (char*)&InputBuffer[myexports[i].function_id],  myexports[i].instruction_pointer);
				}
			}
		}
	}
}
void PrintHelp(void) {
	printf("\n\n"
		   "readrex: File format parser for R3X. - help\n"
	       "Usage  -exe [executable name]\n\n"
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
void ParseArguments(int argc, char* argv[]){
	for(int i = 0; i < argc; i++){
		if(strncmp(argv[i], "-exe", 2)==0){
			if(i+1 >= argc){
				fprintf(stderr, "Error: -exe option, input file not specified\n");
				exit(EXIT_FAILURE);
			}
			InputFile = argv[i+1];
		} else if(strncmp(argv[i], "-h", 2)==0){
			PrintHelp();
			exit(EXIT_SUCCESS);
		}
	}
}
void read_symbol_table(r3x_header_t* header, uint8_t* Memory, uint32_t size, uint32_t IP) {
	(void)IP;
	if(header->r3x_symbols - PROG_EXEC_POINT > size) {
		printf("Error: Corrupted symbol header! Unable to read symbol table!\n");
		return;
	} else {
		int number_of_symbols = header->r3x_symbolsize / sizeof(r3x_symbol_t);
		r3x_symbol_t* mysymbols = (r3x_symbol_t*)&Memory[header->r3x_symbols - PROG_EXEC_POINT];
		for(int i = 0; i < number_of_symbols; i++) {
			if(mysymbols[i].SymbolName - PROG_EXEC_POINT > size) {
				printf("Name: Invalid\n");
			} else {
				printf("Name: %s\n", (char*)&Memory[mysymbols[i].SymbolName - PROG_EXEC_POINT]);
			}
			printf("Symbol start: 0x%X\n", mysymbols[i].SymbolStart);
			printf("Symbol end: 0x%X\n", mysymbols[i].SymbolEnd);
		}
	}
}
