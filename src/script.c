/* A small script parser to parse R3X script (.rxs) files */

#include <r3x_script.h>
#include <nt_malloc.h>

FILE* input_file = NULL;
int look = 0;
unsigned int line_num = 0;
char** string_pointers = NULL;
unsigned int used_string_ptrs = 0;
unsigned int string_pointers_sz = 0;
ScriptState* GlobalScriptState = NULL;
bool IsEnd = false;

typedef enum {
	TOK_NULL,
	TOK_CPUFREQ,
	TOK_FONT,
	TOK_FONTSCALE,
	TOK_FONTHEIGHT,
	TOK_FONTWIDTH,
	TOK_SCREENHEIGHT,
	TOK_SCREENWIDTH,
	TOK_EXEFILE,
	TOK_ARGS,
	TOK_STACKSIZE,
	TOK_END
} tokentype;

void get_char(void);
void eat_blanks(void);
tokentype get_keyword(void);
void do_statement(void);
void expect(char);

void do_args(void);
void do_freq(void);
void do_font(void);
void do_font_h(void);
void do_font_w(void);
void do_font_s(void);
void do_screen_h(void);
void do_screen_w(void);
void do_stack_s(void);
void do_end(void);
void do_exefile(void);

char* return_next_token(void);
char* return_next_string(void);
void cleanup(void);

void cleanup(void) {
	for(unsigned int i = 0; i < string_pointers_sz; i++) {
		if(string_pointers[i]!=NULL) {
			nt_free(string_pointers[i]);
		}
	}
}
void get_char(void) {
	look = fgetc(input_file);
}
void expect(char c) {
	if(look != c) {
		fprintf(stderr, "<%u> script: Error: expected %c, but got %c (0x%X, %u)\n", c, look, (unsigned int)look, (unsigned int)look, line_num);
	}
	get_char();
	eat_blanks();
}
void eat_blanks(void) {
	while(look == '\t' || look == '\r' || look == ' ')
		get_char();
}
char* return_next_token(void) {
	size_t string_size = 0;
	char *string;

	string = nt_malloc (1);
	string[0] = 0;

	for (; look != ' ' && look != '\t' && look != '\n' && look != '/' && look != '*' && look != '+' && look != '-' && look != '^' && look != '%' && look != '@' && look != '{' && look != '}' && look != '[' && look != ']' && look != '^' && look != '&' && look != '|' && look != '!' && look != '~' && look != '\'' && look != ';' && look != '>' && look != '<' && look != '?' && look != '>' && look != ',' && look != '=' && look != '(' && look != ')' && look != '{' && look != '[' && look != ']' && look != '}'; get_char ()) {
		++string_size;
		string = nt_realloc(string, string_size + 1);
		string[string_size - 1] = look;
		string[string_size] = 0;
	}
	eat_blanks();
	return string;
}
char* return_next_string(void) {
	size_t string_size = 0;
	char *string;

	string = nt_malloc(1);
	string[0] = 0;

	for (get_char (); look != '"'; get_char ()) {
		++string_size;
		string = nt_realloc(string, string_size + 1);
		string[string_size - 1] = look;
		string[string_size] = 0;
    }

	get_char ();
	eat_blanks();
	return string;
}
tokentype get_keyword(void) {
	char token[21];
	memset(token, 0, sizeof(token));
	eat_blanks();
	get_char();
	unsigned int i = 0;
	while((isalpha(look) || look == '_') && (i < sizeof(token)-1)) {
		token[i] = look;
		get_char();
		i++;
	}
	token[i] = 0;
	tokentype returnval = TOK_NULL;
	if(!strcmp(token, "cpu_freq")) {
		returnval = TOK_CPUFREQ;
	} else if(!strcmp(token, "font_file")) {
		returnval = TOK_FONT;
	} else if(!strcmp(token, "font_height")) {
		returnval = TOK_FONTHEIGHT;
	} else if(!strcmp(token, "font_width")) {
		returnval = TOK_FONTWIDTH;
	} else if(!strcmp(token, "font_scale")) {
		returnval= TOK_FONTSCALE;
	} else if(!strcmp(token, "screen_height")) {
		returnval = TOK_SCREENHEIGHT;
	} else if(!strcmp(token, "screen_width")) {
		returnval = TOK_SCREENWIDTH;
	} else if(!strcmp(token, "executable")) {
		returnval = TOK_EXEFILE;
	} else if(!strcmp(token, "args")) {
		returnval = TOK_ARGS;
	} else if(!strcmp(token, "stack")) {
		returnval = TOK_STACKSIZE;
	} else if(!strcmp(token, "end")) {
		returnval = TOK_END;
	} else {
		fprintf(stderr, "<%u> script: Expected keyword got %s\n", token, line_num);
		exit(0);
	}
	expect(':');
	return returnval;
}
void do_statement(void) {
	tokentype next_token = get_keyword();
	switch(next_token) {
		case TOK_ARGS:	do_args(); break;
		case TOK_CPUFREQ: do_freq(); break;
		case TOK_END: do_end(); break;
		case TOK_EXEFILE: do_exefile(); break;
		case TOK_FONT: do_font(); break;
		case TOK_FONTHEIGHT: do_font_h(); break;
		case TOK_FONTWIDTH: do_font_w(); break;
		case TOK_FONTSCALE: do_font_s(); break;
		case TOK_SCREENHEIGHT: do_screen_h(); break;
		case TOK_SCREENWIDTH: do_screen_w(); break;
		case TOK_STACKSIZE: do_stack_s(); break;
		default: assert(0); break;
	}
	if(feof(input_file)) {
		IsEnd = true;
	}
	if(IsEnd!=false) {
		expect('\n');
		line_num++;
	}
}
void do_args(void) {
	GlobalScriptState->Args = return_next_string();
}
void do_freq(void) {
	GlobalScriptState->Frequency = atof(return_next_token());
}
void do_end(void) {
	IsEnd = true;
}
void do_exefile(void) {
	GlobalScriptState->ExeName = return_next_string();
}
void do_font(void) {
	GlobalScriptState->FontName = return_next_string();
}
void do_font_h(void) {
	GlobalScriptState->FontHeight = atoi(return_next_token());
}
void do_font_w(void) {
	GlobalScriptState->FontWidth = atoi(return_next_token());
}
void do_font_s(void) {
	GlobalScriptState->FontScale = atof(return_next_token());
}
void do_screen_h(void) {
	GlobalScriptState->ScreenHeight = atoi(return_next_token());
}
void do_screen_w(void) {
	GlobalScriptState->ScreenWidth = atoi(return_next_token());
}
void do_stack_s(void) {
	GlobalScriptState->StackSize = atoi(return_next_token());
}
void parse_script(char* file_name, ScriptState* State) {
	input_file = fopen(file_name, "r");
	if(input_file == NULL) {
		fprintf(stderr, "Error: Cannot find %s script file.\n", file_name);
		exit(EXIT_FAILURE);
	}
	GlobalScriptState = State;
	while(IsEnd == false) {
		do_statement();
	}
}
