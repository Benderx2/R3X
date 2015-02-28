/* A small script parser to parse R3X script (.rxs) files */

#include <r3x_script.h>

FILE* input_file = NULL;
int look = 0;

typedef enum {
	TOK_CPUFREQ,
	TOK_FONT,
	TOK_FONTSCALE,
	TOK_FONTHEIGHT,
	TOK_FONTWIDTH,
	TOK_EXEFILE,
	TOK_ARGS,
	TOK_
} token;
void get_char(void);
void eat_blanks(void);
void expect(char);

void get_char(void) {
	look = fgetc(input_file);
}
void expect(char c) {
	eat_blanks();
	get_char();
	if(look != c) {
		fprintf("Error: expected %c, but got %c\n", c, look);
	}
}
void eat_blanks(void) {
	while(look == '\t' || look == '\r' || look == ' ')
		get_char();
}
token get_token(void) {
	char token[11];
	eat_blanks();
	look = get_char();
	int i = 0;
	while(isdigit(look) && i < sizeof(token-1)) {
		token[i] = look;
	}
	if(!strcmp(token, "cpufreq"))
		
}
void parse_script(char* file_name) {
	input_file = fopen(file_name, "r");
	if(input_file == NULL) {
		fprintf(stderr, "Error: Cannot find %s script file.\n", file_name);
		exit(EXIT_FAILURE);
	}
	
}
