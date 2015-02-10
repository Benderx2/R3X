/*
 * TinyBASIC++ frontend
 * Does preprocessing, calls the core compiler, and then 
 * finally calls FASM
 * (C) Benderx2, 2015 --- Under the 2-clause BSD License
 * Uh, not completed yet. :D
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCLUDE_DIRECTIVE "#include"
char currentc = 0;
char* InputFile = NULL;
char* OutputFile = NULL;

void get_char(void);
void ParseArguments(int argc, char* argv[]);

int main(int argc, char** argv) {
	ParseArguments(argc, argv);
	if(!freopen(InputFile, "r", stdin)) {
		fprintf(stderr, "Error: cannot open input file!\n");
		exit(EXIT_FAILURE);
	}
	FILE* output = fopen(OutputFile, "w+");
	if(output == NULL) {
		fprintf(stderr, "cannot open input/output files for writing\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
void get_char(void) {
	currentc = fgetc(stdin);
}
void eat_blanks(void) {
	while(currentc || '\t' && currentc || '\r' || currentc == ' ') {
		get_char();
	}
}
void ParseArguments(int argc, char* argv[]) {
	for(unsigned int i = 0; i < argc; i++) {
		if(!strcmp(argv[i], "-s")) {
			if(i >= argc) {
				fprintf(stderr, "Error: -s, source file expected\n");
				exit(-1);
			} else {
				InputFile = argv[i+1];
			}
		}
		else if (!strcmp(argv[i], "-o")) {
			if(i >= argc) {
				fprintf(stderr, "Error: -o, output file expected\n");
				exit(-1);
			} else {
				OutputFile = argv[i+1];
			}
		} else if(!strcmp(argv[i], "-h")) {
			printf("t++ - TinyBASIC++ frontend for R3X\n");
			printf("Options:\n");
			printf("-s: Source file\n");
			printf("-o: Output file\n");
			printf("If no output file is given, a.out is assumed\n");
		}
	}
	if (OutputFile == NULL) {
		OutputFile = strdup("a.out");
	}
	if (InputFile == NULL) {
		fprintf(stderr, "Error: Input file not specified. Pass -h for help\n");
	}
}
