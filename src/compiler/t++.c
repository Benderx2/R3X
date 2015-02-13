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
/*#ifndef RX_PREFIX
#error "No RX_PREFIX defined!"
#endif*/

void check_if_preprocessor_keyword(void);

#define INCLUDE_DIRECTIVE "#include"

char look = 0;
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
	check_if_preprocessor_keyword();
	check_if_preprocessor_keyword();
	check_if_preprocessor_keyword();
	check_if_preprocessor_keyword();
	check_if_preprocessor_keyword();
	return 0;
}
void get_char(void) {
	look = fgetc(stdin);
}
void eat_blanks(void) {
	while(look || '\t' && look || '\r' || look == ' ') {
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
void check_if_preprocessor_keyword(void) {
	if(feof(stdin)) {
		return;
	}
	eat_blanks();
	get_char();
	//! Ignore strings.
	if(look == '"') {
		get_char();
		while(look != '"') {
			get_char();
		}
	}
	if(look == '#') {
		//! might be a preprocessor keyword..
		get_char();
		if(look == '!') {
			//! Nah it's a comment... :P
			while(look != '\n') {
				get_char();
			}
			return;
		} else {
			char token[10];
			token[0] = look;
			for(unsigned int i = 1; i < sizeof(token) && isalpha(look); i++) {
				token[i] = look;
			}
			if(!strcmp(token, "#include")) {
				printf("lol");
				exit(0);
			}
		}
	} else {
		while(look != ' ' && look != '\t' && look != '\r') {
			get_char();
		}
	}
}
