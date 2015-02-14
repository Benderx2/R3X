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
#ifndef RX_PREFIX
#error "No RX_PREFIX defined!"
#endif

char* InputFile = NULL;
char* OutputFile = NULL;

char* ConcatenateStrings(char *s1, char *s2);
void ParseArguments(int argc, char* argv[]);

int main(int argc, char** argv) {
	ParseArguments(argc, argv);
	char* asmfile = ConcatenateStrings(OutputFile, ".asm");
	char* ASMOutputFile = ConcatenateStrings(RX_PREFIX, ConcatenateStrings("/", asmfile));
	char* TBCOutputFile = ConcatenateStrings(RX_PREFIX, "/_temp.bas");
	char* GCCOptions = ConcatenateStrings("gcc -E -P -o ", ConcatenateStrings(TBCOutputFile, ConcatenateStrings(" - < ", InputFile)));
	char* TBCOptions = ConcatenateStrings(RX_PREFIX, ConcatenateStrings("/tbc > ", ConcatenateStrings(ASMOutputFile, ConcatenateStrings(" ", TBCOutputFile))));
	char* FASMOptions = ConcatenateStrings(RX_PREFIX, ConcatenateStrings("/fasm ", ConcatenateStrings(ASMOutputFile, ConcatenateStrings(" ./", OutputFile))));
	/*printf("ASMOutputFile: %s\nTBCOutputFile: %s\nGCCOptions: %s\nTBCOptions: %s\nFASMOptions: %s\n", ASMOutputFile, TBCOutputFile, GCCOptions, TBCOptions, FASMOptions);*/
	system(GCCOptions);
	system(TBCOptions);
	system(FASMOptions);
	/**!
		Remove temp files
	**/
	remove(ASMOutputFile);
	remove(TBCOutputFile);
	return 0;
}
char* ConcatenateStrings(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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
			printf("If no output file is given, a.exe is assumed\n");
		}
	}
	if (OutputFile == NULL) {
		OutputFile = strdup("a");
	}
	if (InputFile == NULL) {
		fprintf(stderr, "Error: Input file not specified. Pass -h for help\n");
		exit(EXIT_FAILURE);
	}
}
