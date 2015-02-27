/*
 * Partly rewritten and extended for R3X systems by Benderx2.
 * T++ Compiler.
 * Licensed as below.
 * WHATS NEW:
 * -Added new operatos - &, ^, |
 * -Add support for hexadecimal (0x), octal (0o) and binary (0b) in expressions.
 * -Added support for procedures - function x, 1 etc.
 * -Added support for recursion
 * -Added memory access operators [], {}, <>
 * -Fixed crazy bug with subtraction
 * -Added better goto/gosub statements with support for labels
 * -Added the 'while' keyword and the != operator
 * -Added structs.
 * -Removed ugly BASIC "numbered" lines
 * -Added strings "", and ASCII characters '' to be used in expressions / arguments
 * -Improved code generator.
*/
/*
Luiji's Tiny BASIC Compiler
Copyright (C) 2012 Entertaining Software, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
/*#ifndef RX_PREFIX
#error "No RX_PREFIX defined!"
#endif*/
#ifdef __GNUC__
# define PRINTF(f, va) __attribute__ ((__format__ (__printf__, f, va)))
#else
# define PRINTF(f, va) /* unsupported */
#endif

#if defined (__STDC__) || defined (_AIX) \
    || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
    || defined (WIN32) || defined (__cplusplus)
# define PARAMS(protos) protos
#else
# define PARAMS(protos) (/* unsupported */)
#endif

#define MAX_WHILE_NESTING 256
enum
{
  T_NULL = 0,
  T_PRINT,
  T_IF,
  T_GOTO,
  T_INPUT,
  T_LET,
  T_GOSUB,
  T_RETURN,
  T_REM,
  T_RUN,
  T_ASM,
  T_LABEL,
  T_WHILE,
  T_ENDW,
  T_FUNC,
  T_ENDF,
  T_STRUCT,
  T_ENDSTRUCT,
  T_GLOBAL,
  T_END
};

enum
{
  O_EQUAL,
  O_NOT_EQUAL,
  O_LESS,
  O_MORE,
  O_LESS_OR_EQUAL,
  O_MORE_OR_EQUAL
};
typedef enum {
	RX_INT8,
	RX_INT16,
	RX_INT32
} RX_STRUCT_TYPE;
typedef struct {
	char** name;
	unsigned int id;
	bool is_used;
} variable_type;
typedef struct {
	char* name;
	unsigned int number_of_arguments;
	bool used;
} function_type;
typedef struct {
	char* Name;
	RX_STRUCT_TYPE Type;
} sub_members;
typedef struct {
	char* Name;
	sub_members* SubMembers;
	unsigned int NumberOfMembers;
} struct_type;
typedef struct {
	struct_type* Structs;
	unsigned int CurrentIndex;
	unsigned int NumberOfStructs;
} s_table;
typedef struct {
	function_type* functions;
	unsigned int total_functions;
	unsigned int current_index;
} f_table;
typedef union {
	int ReturnVal;
	float Float;
} FloatTypeCastInt;
static char        look          = 0;
static int         line          = 1;
static unsigned int			whilelines 	  = 0;
static unsigned int 			currentwhile = MAX_WHILE_NESTING;
static int         use_print_i   = 0;
static int         use_print_s   = 0;
static int         use_print_t   = 0;
static int         use_print_n   = 0;
static int         use_input_i   = 0;
static int			while_stack[MAX_WHILE_NESTING+1] = { 0 };
static int 		stuff_pushed_to_stack = 0;
static bool			returned_float = false;
static int 		number_of_util_regs = 4;
static f_table*    function_table = NULL;
static s_table*	structs_table = NULL;
static char* 		current_function_name = NULL;
static char **     strings       = NULL;
static unsigned int 		current_variable_index = 0;
static unsigned int		total_variables = 0;
static char** 			variables_used = NULL;
static int         string_count  = 0;
static const char *program_name  = "<?>";
static const char *input_name    = NULL;
static const char *output_name   = NULL;
static char* compiler_init_code = NULL;

static void  shutdown      PARAMS ((void));
static void *xmalloc       PARAMS ((size_t size));
static void *xrealloc      PARAMS ((void *original, size_t size));
static void  xfree         PARAMS ((void *pointer));
static void  parse_opts    PARAMS ((int argc, char **argv));
static void  print_help    PARAMS ((void));
static void  print_version PARAMS ((void));
static void  begin         PARAMS ((void));
static void  finish        PARAMS ((void));
static void  error         PARAMS ((const char *format, ...)) PRINTF (1, 2);
static void  eat_blanks    PARAMS ((void));
static void  match         PARAMS ((int what));
static void  get_char      PARAMS ((void));
static char  get_name      PARAMS ((void));
static int   get_num       PARAMS ((void));
static int   get_keyword   PARAMS ((void));
static int   do_line       PARAMS ((void));
static void  do_statement  PARAMS ((void));
static void  do_print      PARAMS ((void));
static void  do_print_item PARAMS ((void));
static void  do_if         PARAMS ((void));
static void  do_goto       PARAMS ((void));
static void  do_input      PARAMS ((void));
static void  do_let        PARAMS ((void));
static void  do_gosub      PARAMS ((void));
static void  do_return     PARAMS ((void));
static void  do_rem        PARAMS ((void));
static void  do_end        PARAMS ((void));
static void  do_expression PARAMS ((void));
static bool  do_term       PARAMS ((void));
static bool  do_factor     PARAMS ((void));
static void  do_string     PARAMS ((void));
static void  do_asm	   PARAMS ((void));
static void  do_while	   PARAMS ((void));
static void  do_endw	   PARAMS ((void));
static void  do_alloc	   PARAMS ((void));
static void  add_variable  PARAMS ((char*));
static void  do_func       PARAMS ((void));
static void  do_struct	   PARAMS ((void));
static void  do_global	   PARAMS ((void));
static void  do_endf		PARAMS ((void));
char* return_next_int_name_and_add (void);
static void  do_function_call PARAMS((void));

static RX_STRUCT_TYPE  do_typecast   PARAMS ((int));

static void  generate_identifier PARAMS((void));

static char*  return_next_int_name PARAMS((void));

char* return_str PARAMS ((void));

char* return_next_tok(void);

static function_type* return_function_type_if_function_exists PARAMS((char*));

static void AddStruct(char* Name, unsigned int NumberOfMembers);

static struct_type* return_struct_type_if_struct_exists(char* StructName);

static RX_STRUCT_TYPE return_type_if_struct_member_exists(char* StructName, char* MemberName);

static unsigned int return_member_offset(char* StructName, char* MemberName);

static void write_init_code	 PARAMS ((const char *format, ...)) PRINTF (1, 2);

/*!
 * Standard C main function
 * */
int
main (argc, argv)
     int argc;
     char **argv;
{
  atexit (shutdown);

  parse_opts (argc, argv);
	
  variables_used = xmalloc(sizeof(char**)*16);
  total_variables = 16;
  
  function_table = xmalloc(sizeof(f_table));
  function_table->functions = xmalloc(sizeof(function_type)*16);
  function_table->current_index = 0;
  function_table->total_functions = 16;
  
  structs_table = xmalloc(sizeof(s_table));
  structs_table->Structs = xmalloc(sizeof(struct_type)*16);
  structs_table->CurrentIndex = 0;
  structs_table->NumberOfStructs = 16;
  
  begin ();
  while (do_line ());
  finish ();

  return EXIT_SUCCESS;
}

/*!
 * Adds a variable to the variable table with a name.
 * */
static bool
check_if_variable_exists(char* var_name)
{
	unsigned int i;
	for(i = 0; i < total_variables; i++) {
		if(variables_used[i] != NULL) {
			if(!strcmp(variables_used[i], var_name)) {
				//! Already used variable
				return true;
			}
		}
	}
	return false;
}
static void 
add_variable(char* var_name) {
	unsigned int i;
	for(i = 0; i < total_variables; i++) {
		if(variables_used[i] != NULL) {
			if(!strcmp(variables_used[i], var_name)) {
				//! Already used variable
				return;
			}
		}
	}
	if(total_variables < current_variable_index-1) {
		total_variables += 16;
		variables_used = xrealloc(variables_used, total_variables * sizeof(char*));
	}
	variables_used[current_variable_index] = var_name;
	current_variable_index++;
}
static void
add_to_function_table(char* function_name, unsigned int no_of_args) {
	function_type* type = return_function_type_if_function_exists(function_name);
	if(type!=NULL) {
		error("function %s already defined\n", function_name);
	}
	if(function_table->total_functions < function_table->current_index-1) {
		function_table->total_functions += 16;
		function_table->functions = xrealloc(function_table->functions, function_table->total_functions * sizeof(function_type));
	}
	function_table->functions[function_table->current_index].name = strdup(function_name);
	function_table->functions[function_table->current_index].number_of_arguments = no_of_args;
	function_table->functions[function_table->current_index].used = true;
	function_table->current_index++;
}
static function_type*
return_function_type_if_function_exists(char* function_name) {
	unsigned int i; 
	for(i = 0; i < function_table->total_functions; i++) {
		if(function_table->functions[i].used == true) {
			if(!strcmp(function_table->functions[i].name, function_name)) {
				return &function_table->functions[i];
			}
		}
	}
	return NULL;
}
static void AddStruct(char* Name, unsigned int NumberOfMembers) {
	structs_table->CurrentIndex++;
	if(Name==NULL) {
		error("Name of struct is: (null)\n");
	}
	struct_type* temp = return_struct_type_if_struct_exists(Name);
	if(temp!=NULL) {
		error("struct %s redefined.\n", Name);
	}
	if(structs_table->NumberOfStructs < structs_table->CurrentIndex-1) {
		structs_table->NumberOfStructs += 16;
		structs_table->Structs = xrealloc(structs_table->Structs, structs_table->NumberOfStructs * sizeof(struct_type));
	}
	structs_table->Structs[structs_table->CurrentIndex].Name = Name;
	structs_table->Structs[structs_table->CurrentIndex].NumberOfMembers = NumberOfMembers;
	structs_table->Structs[structs_table->CurrentIndex].SubMembers = xmalloc(NumberOfMembers*sizeof(sub_members));
}
static void AddStructMember(char* Name, RX_STRUCT_TYPE Type, unsigned int Index) {
	/** 
	 * Add a member to struct
	**/
	if(Index >= structs_table->Structs[structs_table->CurrentIndex].NumberOfMembers) {
	    error("More definitions than expected in struct: %s\n", structs_table->Structs[structs_table->CurrentIndex].Name);
	}
	structs_table->Structs[structs_table->CurrentIndex].SubMembers[Index].Type = Type;
	structs_table->Structs[structs_table->CurrentIndex].SubMembers[Index].Name = Name;
}
static struct_type* return_struct_type_if_struct_exists(char* StructName) {
	unsigned int i = 0;
	for(i = 0; i < structs_table->NumberOfStructs; i++) {
		if(structs_table->Structs[i].Name != NULL) {
			if(!strcmp(structs_table->Structs[i].Name, StructName)) {
				return &(structs_table->Structs[i]);
			}
		}
	}
	return NULL;
}
static RX_STRUCT_TYPE return_type_if_struct_member_exists(char* StructName, char* MemberName) {
    struct_type* StructType = return_struct_type_if_struct_exists(StructName);
    if(StructType == NULL) {
      error("Struct %s doesn't exist!\n", StructName);
    }
    unsigned int i = 0;
    for(i = 0; i < StructType->NumberOfMembers; i++) {
      if(!strcmp(StructType->SubMembers[i].Name, MemberName)) {
	return StructType->SubMembers[i].Type;
      }
    }
    error("struct member %s, doesn't exist in struct %s\n", MemberName, StructName);
    //! just here to skip warnings, code below wont be executed.
    return RX_INT8;
}
static unsigned int return_member_offset(char* StructName, char* MemberName) {
    struct_type* StructType = return_struct_type_if_struct_exists(StructName);
    if(StructType == NULL) {
      error("Struct %s doesn't exist!\n", StructName);
    }
    unsigned int i = 0;
    unsigned int OffSet = 0;
    for(i = 0; i < StructType->NumberOfMembers; i++) {
      if(!strcmp(StructType->SubMembers[i].Name, MemberName)) {
	return OffSet;
      }
      if(StructType->SubMembers[i].Type == RX_INT8) {
	OffSet += 1;
      } else if(StructType->SubMembers[i].Type == RX_INT32) {
	OffSet += 4;
      } else if(StructType->SubMembers[i].Type == RX_INT16) {
	OffSet += 2;
      }
    }
    error("struct member %s, doesn't exist in struct %s\n", MemberName, StructName);
    //! just here to skip warnings, code below wont be executed.
    return 0;
}
static unsigned int return_sizeof_struct(char* StructName) {
  struct_type* StructType = return_struct_type_if_struct_exists(StructName);
    if(StructType == NULL) {
      error("Struct %s doesn't exist!\n", StructName);
    }
    unsigned int i = 0;
    unsigned int OffSet = 0;
    for(i = 0; i < StructType->NumberOfMembers; i++) {
      if(StructType->SubMembers[i].Type == RX_INT8) {
	OffSet += 1;
      } else if(StructType->SubMembers[i].Type == RX_INT32) {
	OffSet += 4;
      } else if(StructType->SubMembers[i].Type == RX_INT16) {
	OffSet += 2;
      }
    }
    return OffSet;
}
/*!
 * Frees memory allocated previously.
 * */
static void
shutdown ()
{
  int i;

  for (i = 0; i < string_count; ++i)
    xfree (strings[i]);

  xfree (strings);
}
/*!
 * A wrapper over malloc
 * */
static void *
xmalloc (size)
     size_t size;
{
  void *data = malloc (size);
  if (!data)
    {
      fprintf (stderr, "%s: failed to allocate memory\n", program_name);
      exit (EXIT_FAILURE);
    }
  memset(data, 0, size);
  return data;
}
/*!
 * A wrapper over realloc
 * */
static void *
xrealloc (original, size)
     void *original;
     size_t size;
{
  void *data = realloc (original, size);
  if (!data)
    {
      fprintf (stderr, "%s: failed to allocate memory\n", program_name);
      exit (EXIT_FAILURE);
    }
  return data;
}
/*!
 * A wrapper over free
 * */
static void
xfree (pointer)
     void *pointer;
{
  if (pointer)
    free (pointer);
}
/*!
 * Parses arugments given to program
 * */
static void
parse_opts (argc, argv)
     int argc;
     char **argv;
{
  extern char *optarg;
  extern int optind;
  int getopt PARAMS ((int argc, char *const *argv, const char *optstring));

  int was_error = 0, opt;

  program_name = strrchr (argv[0], '/');
  if (program_name)
    ++program_name;
  else
    program_name = argv[0];

  while ((opt = getopt (argc, argv, "hVo:")) != -1)
    switch (opt)
      {
      case 'h':
	print_help ();
	exit (EXIT_SUCCESS);
      case 'V':
	print_version ();
	exit (EXIT_SUCCESS);
      case 'o':
	output_name = optarg;
	break;
      case '?':
	was_error = 1;
	break;
      default:
	assert (0);
	break;
      }

  if (was_error)
    exit (EXIT_FAILURE);

  if (optind < argc)
    input_name = argv[optind];

  for (++optind; optind < argc; ++optind)
    fprintf (stderr, "%s: ignoring argument '%s'\n", program_name,
	     argv[optind]);

  if (input_name)
    {
      if (!freopen (input_name, "r", stdin))
	{
	  fprintf (stderr, "%s: failed to open for reading\n", input_name);
	  exit (EXIT_FAILURE);
	}
    }
  else
    {
      input_name = "<stdin>";
    }

  if (output_name)
    {
      if (!freopen (output_name, "w", stdout))
	{
	  fprintf (stderr, "%s: failed to open for writing\n", output_name);
	  exit (EXIT_FAILURE);
	}
    }
  else
    {
      output_name = "<stdout>";
    }
}
/*!
 * Returns a string literal
 * */
char* 
return_str() {
  int i;
  size_t string_size = 0;
  char *string;

  string = xmalloc (1);
  string[0] = 0;

  for (get_char (); look != '"'; get_char ())
    {
      ++string_size;
      string = xrealloc (string, string_size + 1);
      string[string_size - 1] = look;
      string[string_size] = 0;
    }

  get_char ();
  eat_blanks();
  return string;
}
/*!
 * Returns a new token separated by operators etc.
 * */
char* 
return_next_tok() {
  int i;
  size_t string_size = 0;
  char *string;

  string = xmalloc (1);
  string[0] = 0;

  for (; look != ' ' && look != '\t' && look != '\n' && look != '/' && look != '*' && look != '+' && look != '-' && look != '^' && look != '%' && look != '@' && look != '{' && look != '}' && look != '[' && look != ']' && look != '^' && look != '&' && look != '|' && look != '!' && look != '~' && look != '\'' && look != ';' && look != '.' && look != '>' && look != '<' && look != '?' && look != '>' && look != ',' && look != '=' && look != '(' && look != ')' && look != '{' && look != '[' && look != ']' && look != '}'; get_char ())
    {
      ++string_size;
      string = xrealloc (string, string_size + 1);
      string[string_size - 1] = look;
      string[string_size] = 0;
    }
  eat_blanks();
  return string;
}
/*!
 * Returns a variable name.
 * */
char* return_next_int_name()
{
	char* returnval = return_next_tok();
	//! Don't mess with keywords!
	if(!strcmp(returnval, "int32_ptr") || (!strcmp(returnval, "int8_ptr")) || !(strcmp(returnval, "int16_ptr")) || (!strcmp(returnval, "mul_f")) || (!strcmp(returnval, "add_f")) || (!strcmp(returnval, "sub_f")) || (!strcmp(returnval, "div_f")) || (!strcmp(returnval, "conv_f")) || (!strcmp(returnval, "conv_i")) || (!strcmp(returnval, "mod_f")) || (!strcmp(returnval, "left_shift")) || (!strcmp(returnval, "right_shift")) || (!strcmp(returnval, "not")) || (!strcmp(returnval, "neg")) || (!strcmp(returnval, "sizeof")) || !strcmp(returnval, "alloc")) {
		return returnval;
	}
	if(current_function_name != NULL) {
		char* new_val = xmalloc(strlen(returnval)+strlen(current_function_name)+3);
		strcpy(new_val, current_function_name);
		new_val[strlen(new_val)] = '.';
		strcat(new_val, returnval);
		returnval = new_val;
	}
	if(check_if_variable_exists(returnval)==false) {
	  error("Variable %s not declared!\n", returnval);
	}
	eat_blanks();
	return returnval;
}
char* return_next_int_name_and_add()
{
	char* returnval = return_next_tok();
	//! Don't mess with keywords!
	if(!strcmp(returnval, "int32_ptr") || (!strcmp(returnval, "int8_ptr")) || !(strcmp(returnval, "int16_ptr")) || (!strcmp(returnval, "mul_f")) || (!strcmp(returnval, "add_f")) || (!strcmp(returnval, "sub_f")) || (!strcmp(returnval, "div_f")) || (!strcmp(returnval, "conv_f")) || (!strcmp(returnval, "conv_i")) || (!strcmp(returnval, "mod_f")) || (!strcmp(returnval, "left_shift")) || (!strcmp(returnval, "right_shift")) || (!strcmp(returnval, "not")) || (!strcmp(returnval, "neg")) || (!strcmp(returnval, "sizeof")) || !strcmp(returnval, "alloc")) {
		return returnval;
	}
	if(current_function_name != NULL) {
		char* new_val = xmalloc(strlen(returnval)+strlen(current_function_name)+3);
		strcpy(new_val, current_function_name);
		new_val[strlen(new_val)] = '.';
		strcat(new_val, returnval);
		returnval = new_val;
	}
	if(check_if_variable_exists(returnval)==false) {
	  add_variable(returnval);
	}
	eat_blanks();
	return returnval;
}
char* add_var_with_context(char* var_name) {
	char* new_val = xmalloc(strlen(var_name)+strlen(current_function_name)+3);
	if(current_function_name != NULL) {
		strcpy(new_val, current_function_name);
		new_val[strlen(new_val)] = '.';
		strcat(new_val, var_name);
	}
	add_variable(new_val);
}
/*!
 * Skips spaces, tabs, and CR
 */
static void
eat_blanks ()
{
  while (look == ' ' || look == '\t' || look == '\r')
    get_char ();
}
/*!
 * Checks if a certain character is at current position in source
 */
static void
match (what)
     int what;
{
  if (look == what)
    get_char ();
  else
    error ("expected '%c' got '%c'", what, look);

  eat_blanks ();
}
/*!
 * Gets a character from the source file
 */
static void
get_char ()
{
  look = fgetc (stdin);
}
/*!
 * Gets an integer literal, decimal/hex/octal
 */
static int
get_num ()
{
  int result = 0;
  returned_float = false;
  if(look == '0') {
	char operand_prefix = getc(stdin);
	operand_prefix = tolower(operand_prefix);
	if(operand_prefix == 'x') {
	   get_char();
	   for(; isdigit(look) || look == 'A' || look == 'B' || look == 'C' || look == 'D' || look == 'E' || look == 'F'; get_char())
	   {
		   result *= 16;
		   if(isdigit(look)) {
			result += look - '0';
		   } else {
			switch(look) {
				case 'A':
					result += 10;
					break;
				case 'B':
					result += 11;
					break;
				case 'C':
					result += 12;
					break;
				case 'D':
					result += 13;
					break;
				case 'E':
					result += 14;
					break;
				case 'F':
					result += 15;
					break;
				default:
					error("expected hex digit but got %c\n", look);
					break;
			}
		   }
	   }
	   eat_blanks();
	   return result;
	} else if(operand_prefix == 'o'){
		get_char();
		for(; isdigit(look); get_char()) {
			result *= 8;
			int octal = look - '0';
			if(octal >= 8) {
				error("expect octal digit but got %c\n", look);
			}
			result += octal;
		}
		eat_blanks();
		return result;
	} else if(operand_prefix == 'b') {
		get_char();
		for(; look == '0' || look == '1'; get_char()) {
			result *= 2;
			result += look - '0';
		}
		eat_blanks();
		return result;
	} else {
		ungetc(operand_prefix, stdin);
	}
  }
  if (isdigit (look))
    for (; isdigit (look) || look == '.'; get_char ())
      {
	if(look == '.') {
			get_char();
			FloatTypeCastInt TempTypeCast;
			//! floatin point shit
			//! result stores the stuff, convert it to floating point IEEE-754 system or whatever....
			float temp = result;
			float divider = 10;
			for(; isdigit(look); get_char()) {
				temp += (look - '0') / divider;
				divider = divider * 10;
			}
			TempTypeCast.Float = temp;
			returned_float = true;
			return TempTypeCast.ReturnVal;
	} else {
		result *= 10;
		result += look - '0';
	}
      }
  else
    error ("expected integer got '%c'", look);

  eat_blanks ();

  return result;
}
/*! 
 * Parses an expression
 */
static void
do_expression ()
{
  if (look == '+' || look == '-')
    puts ("\tloadr R1, 0");
  else
    do_term ();
  bool current_expression_contains_float = false;
  while (look == '+' || look == '-')
    {
      int op = look;
      puts ("\tpushar R1");
      match (look);
      do_term ();
      puts ("\tpopar R2");
      if (op == '+')
	puts ("\tpushr R1\n\tpushr R2\n\tadd\n\tpopr R1\n\tpopn 2\n");
      else if (op == '-')
	puts ("\tpushr R2\n\tpushr R1\n\tsub\n\tpopr R1\n\tpopn 2\n"); 
    }
}
/*!
 * Provides support for non-separing operators (multiplication, division, modulo bitwise etc.)
 */
static bool
do_term ()
{
  bool return_val = false;
  do_factor ();
  while (look == '&' || look == '^' || look == '|' || look == '*' || look == '/' || look == '%')
    {
      int op = look;
      puts ("\tpushar R1");
      match (look);
      do_factor ();
      puts ("\tpopar R2");
      if (op == '*') {
		puts ("\tpushr R1\n\tpushr R2\n\tmul\n\tpopr R1\n\tpopn 2");
	  }
      else if (op == '/') {
		puts ("\tpushr R2\n\tpushr R1\n\tdiv\n\tpopr R1\n\tpopn 2");
	  }
	  else if (op == '&') {
		puts ("\tpushr R2\n\tpushr R1\n\tand\n\tpopr R1\n\tpopn 2");
	  }
	  else if (op == '|') {
		puts ("\tpushr R2\n\tpushr R1\n\tor\n\tpopr R1\n\tpopn 2");
	  }
	  else if (op == '^') {
		puts ("\tpushr R2\n\tpushr R1\n\txor\n\tpopr R1\n\tpopn 2");
	  }
	  else if (op == '%'){
		puts("pushr R2\n\tpushr R1\n\tmod\n\tpopr R1\n\tpopn 2");
	  } 
    }
    return return_val;
}
/*!
 * Factors an expression, parses parenetheses and memory access brackets.
 */
static bool
do_factor ()
{
  if (look == '(')
    {
      match ('(');
      do_expression ();
      match (')');
    } else if(look == '<') {
      match('<');
      do_typecast(0);
    }
  else if (isalpha (look))
    {
	  generate_identifier();
    }
  else if (look == '$') {
	get_char();
	if(current_function_name == NULL) {
		error("Cannot use arguments when not in a function\n");
	}
	function_type* type = return_function_type_if_function_exists(current_function_name);
	if(type==NULL) {
		error("INTERNAL COMPILER ERROR!\n");
	}
	do_expression();
	printf("\tpushr R1\n");
	printf("\tpush 1\n");
	printf("\tsub\n");
	printf("\tpopr R1\n");
	//!printf("\tpopn 2\n");
	printf("\tpush %u\n", type->number_of_arguments);
	printf("\tpush %u\n", stuff_pushed_to_stack);
	printf("\tadd\n");
	printf("\tpopr R6\n");
	//!printf("\tpopn 2\n");
	printf("\tpushr R6\n");
	printf("\tpush %u\n", number_of_util_regs);
	printf("\tadd\n");
	printf("\tpopr R6\n");
	//!printf("\tpopn 2\n");
	printf("\tpushr R6\n");
	printf("\tpushr R1\n");
	printf("\tsub\n");
	printf("\tpopr R1\n");
	//!printf("\tpopn 2\n");
	printf("\tpopn 8\n");
	//!printf("\tloadr R8, %u\n", (type->number_of_arguments + stuff_pushed_to_stack + number_of_util_regs) - (get_num()-1));
	printf("\tloadrr R8, R1\n");
	printf("\tloadsr R8\n");
	printf("\tpopr R1\n");
  }
  else if(look == '\'') {
	get_char();
	if(look == '\'') {
		printf("\tloadr R1, 0\n");
	}
	else {
		int num_char = look;
		printf("\tloadr R1, %i\n", num_char);
		get_char();
		match('\'');
	}
  }
  else if(look == '@') {
		get_char();
		do_function_call();
  }
  else if(look == '"') {
		do_string ();
	}
  else
    {
      printf ("\tloadr R1, %i\n", get_num ());
      return returned_float;
    }
    return false;
}
/*!
 * Generates code for string declaration
 */
static void
do_string ()
{
  int i;
  size_t string_size = 0;
  char *string;

  string = xmalloc (1);
  string[0] = 0;

  for (get_char (); look != '"'; get_char ())
    {
      ++string_size;
      string = xrealloc (string, string_size + 1);
      string[string_size - 1] = look;
      string[string_size] = 0;
    }

  get_char ();

  for (i = 0; i < string_count; ++i)
    if (!strcmp (strings[i], string))
      {
	printf ("\tloadr R1, s%i\n", i);
	return;
      }

  ++string_count;
  strings = xrealloc (strings, string_count * sizeof (char *));
  strings[string_count - 1] = string;

  printf ("\tloadr R1, s%i\n", i);
}
/*!
 * Outputs help
 * */
static void
print_help ()
{
  printf ("Usage: %s [-o OUTPUT] [INPUT]\n", program_name);
  puts ("");
  puts ("");
  puts ("Options:");
  puts ("  -o  set output file name");
  puts ("  -h  print this help message and exit");
  puts ("  -V  print this version message adn exit");
  puts ("");
  puts ("Input and output defaults to stdin and stdout.");
  puts ("");
  puts ("Report bugs to: Issues at github repository");
  puts ("Project Page: http://github.com/Benderx2/R3X");
}
/*!
 * Outputs version
 * */
static void
print_version ()
{
  puts ("T++ implementation of R3X (Forked from Luiji's tiny basic compiler)");
  puts ("Copyright (C) 2012 Entertaining Software, Inc. (Orginal Copyright Notice)");
  puts ("License GPLv3+: GNU GPL version 3 or later \
<http://gnu.org/licenses/gpl.html>");
  puts ("This is free software: you are free to change and redistribute it.");
  puts ("There is NO WARRANTY, to the extent permitted by law.");
  puts ("Modified by Benderx2 to run on R3X Systems.\n");
}
/*!
 * Adds system specific functions, like print, alloc, input etc.
 * */
static void
begin ()
{

  puts ("");
  puts ("include 'libR3X/libR3X.pkg'");
  /*
   * 256-bit header in text section
   * */
  puts (".text {");
  puts ("\tdd 0x56081124");
  puts ("\tdd 0x12345678");
  puts ("\tdd 0x12335850");
  puts ("\tdd 0xFFFF3FFF");
  puts ("\tdd 0x23FF0FFF");
  puts ("\tdd 0x13370000");
  puts ("\tdd 0x66600000");
  puts ("\tdd 0xEF7E0016");
  puts ("}");
  puts ("");
  puts (".text {");
  puts ("");

  get_char ();
}
/*!
 * Adds system specific mechanism, called when the compiler finishes parsing code.
 * */
static void
finish ()
{
  int i;

  puts ("");
  puts ("; exit to operating system");
  puts ("");
  puts ("_exit:");
  puts ("\tConsole.WaitKey");
  puts ("\tSystem.Quit 0");
  puts ("");

  if (use_print_i)
    {
      puts ("; print an integer to the terminal");
      puts ("print_i:");
      puts ("\tpushr R1");
      puts ("\tsyscall SYSCALL_PUTI");
      puts ("\tpopr R1");
      puts ("\tret");
    }
  else
    {
      puts ("; print_i excluded");
    }
  puts ("");

  if (use_print_s)
    {
      puts ("; print string to terminal");
      puts ("print_s:");
      puts ("\tpushr R1");
      puts ("\tsyscall SYSCALL_PUTS");
      puts ("\tpopr R1");
      puts ("\tret");
    }
  else
    {
      puts ("; print_s excluded");
    }
  puts ("");

  if (use_print_t)
    {
      puts ("; print a tab to the terminal");
      puts ("print_t:");
      puts ("\tpush 0x09");
      puts ("\tsyscall SYSCALL_PUTCH");
      puts ("\tpop");
      puts ("\tret");
    }
  else
    {
      puts ("; print_t excluded");
    }
  puts ("");

  if (use_print_n)
    {
      puts ("; print a newline to the terminal");
      puts ("print_n:");
	  puts ("\tpush 0x0A");
	  puts ("\tsyscall SYSCALL_PUTCH");
	  puts ("\tpop");
	  puts ("\tret");
    }
  else
    {
      puts ("; print_n excluded");
    }
  puts ("");

  if (use_input_i)
    {
      puts ("\t; read a number from the terminal");
      puts ("input_i:");
      puts ("\tsyscall SYSCALL_GETC");
      puts ("\tpush 0");
      puts ("\tcmp");
      puts ("\tpop");
      puts ("\tpopr R1");
      puts ("\tje input_i");
      puts ("\tret");
    }
  else
    {
      puts ("; input_i excluded");
    }
  
  puts ("; Allocate n bytes of memory");
  puts ("alloc_n:");
  puts ("\tpushr R1");
  puts ("\tsyscall SYSCALL_ALLOC");
  puts ("\tpopr R1");
  puts ("\tpop");
  puts ("\tret");
  
  puts("; Output an IEEE-754 floating point number to stdout");
  puts("print_f:");
  puts("\tpushr R1");
  puts("\tsyscall SYSCALL_PUTF");
  puts("\tpopr R1");
  puts("\tret");
  
  puts ("; Free an allocated region");
  puts ("free:");
  puts ("\tpushr R1");
  puts ("\tsyscall SYSCALL_FREE");
  puts ("\tpop");
  puts ("\tret");
  
  puts("; __internal_init = compiler specific program initialization");
  puts("__internal_init:");
  puts("\tret");
  
  puts ("");
  puts ("}");
  for (i = 0; i < total_variables; ++i) {
	    if(variables_used[i] != NULL) {
			printf("\tdeclare_bss_entry v%s\n", variables_used[i]);
		}
  }
  puts (".data {");
  puts ("");

  for (i = 0; i < string_count; ++i)
    printf ("\ts%i: db \"%s\", 0\n", i, strings[i]);

  puts ("}");
  puts("end");
  puts ("; Task Completed -- Assemble with FASM ");
}
/*!
 * Output error when the compiler encounters something unexpected
 * */
static void
error (format)
     const char *format;
     /* ... */
{
  va_list args;

  fprintf (stderr, "%s:%i: error: ", input_name, line);

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  fputs ("\n", stderr);

  exit (EXIT_FAILURE);
}
static void 
write_init_code(format)
  const char* format;
  /* ... */
{
  va_list args;
  va_start(args, format);
  unsigned int number_of_bytes_to_print = vsnprintf(NULL, 0, format, args);
  char* temp = xmalloc(number_of_bytes_to_print+1);
  vsnprintf(temp, number_of_bytes_to_print, format, args);
  compiler_init_code = xrealloc(compiler_init_code, strlen(compiler_init_code)+strlen(temp)+1);
  strcat(compiler_init_code, temp);
  xfree(temp);
}
/*!
 * Gets a keyword, errors out if no keyword is found
 */
static int
get_keyword ()
{
  int i;
  char token[11];
  while(look == ' ' || look == '\t' || look == '\n'){
	get_char();
  }
  if ((!isalpha (look)) && look != '#')
    error ("expected keyword got %c", look);

  for (i = 0; i < (int) sizeof (token) - 1 && (isalpha (look) || look == '#' || look == '!'); ++i)
    {
      token[i] = tolower (look);
      get_char ();
    }

  token[i] = 0;

  if (!strcmp (token, "print"))
    i = T_PRINT;
  else if (!strcmp (token, "if"))
    i = T_IF;
  else if (!strcmp (token, "goto"))
    i = T_GOTO;
  else if (!strcmp (token, "input"))
    i = T_INPUT;
  else if (!strcmp (token, "let"))
    i = T_LET;
  else if (!strcmp (token, "gosub"))
    i = T_GOSUB;
  else if (!strcmp (token, "return"))
    i = T_RETURN;
  else if (!strcmp (token, "rem") || !strcmp(token, "#!"))
    i = T_REM;
  else if (!strcmp (token, "run"))
    i = T_RUN;
  else if (!strcmp (token, "end"))
    i = T_END;
  else if (!strcmp(token, "asm"))
	i = T_ASM;
  else if (!strcmp(token, "label"))
	i = T_LABEL;
  else if(!strcmp(token, "while"))
	i = T_WHILE;
  else if(!strcmp(token, "endw"))
	i = T_ENDW;
  else if (!strcmp(token, "function"))
	i = T_FUNC;
  else if (!strcmp(token, "endf"))
	i = T_ENDF;
  else if(!strcmp(token, "struct"))
	i = T_STRUCT;
  else if(!strcmp(token, "global"))
	i = T_GLOBAL;
  else
    error ("expected keyword got '%s'", token);
  eat_blanks ();

  return i;
}
/*!
 * The compiler calls this when it expects newline(s)
 */
static int
do_line ()
{
  while (look == '\n')
    {
      ++line;
      get_char ();
    }

  if (feof (stdin))
    return 0;

  if (tolower (look) == 'r')
    {
      get_char ();
      if (tolower (look) == 'u')
	{
	  get_char ();
	  if (tolower (look) == 'n')
	    return 0;
	  else
	    error ("expected statement or 'run'");
	}

      ungetc (look, stdin);
      look = 'r';
    }

  do_statement ();

  if (look != '\n')
    error ("expected newline got '%c'", look);
  else
    get_char ();

  ++line;

  return 1;
}
static
void generate_identifier(void) {
	char* next_potential_maccess_operator = return_next_int_name();
	if(!strcmp(next_potential_maccess_operator, "int32_ptr") || (!strcmp(next_potential_maccess_operator, "int8_ptr")) || !(strcmp(next_potential_maccess_operator, "int16_ptr"))) {
		char maccess_operator = next_potential_maccess_operator[3];
		match('(');
		do_expression();
		match(')');
		switch(maccess_operator) {
		case '1':
			printf("\tpushr R0\n\tloadrr R0, R1\n\tlodsw\n\tpopr R0\n");
			break;
		case '3':
			printf("\tpushr R0\n\tloadrr R0, R1\n\tlodsd\n\tpopr R0\n");
			break;
		case '8':
			printf("\tpushr R0\n\tloadrr R0, R1\n\tlodsb\n\tpopr R0\n");
			break;
		default:
			puts("; Warning: Internal compiler error. Unable to find maccess operator!");
			puts ("\tpushr R0\n\tloadrr R0, R1\n\tlodsb\n\tpopr R0\n");
			break;
		}
	  } else if(!strcmp(next_potential_maccess_operator, "mul_f") || !strcmp(next_potential_maccess_operator, "add_f") || !strcmp(next_potential_maccess_operator, "sub_f") || !strcmp(next_potential_maccess_operator, "div_f")) {
			char do_op = next_potential_maccess_operator[0];
			char op_sec = next_potential_maccess_operator[1];
			match('(');
			puts("\tpushar R9");
			do_expression();
			puts("\tloadrr R9, R1");
			match(',');
			do_expression();
			match(')');
			switch(do_op) {
				case 'm':
					if(op_sec == 'u') {
						puts("\tpushr R9\n\tpushr R1\n\tfmul\n\tpopr R1\n\tpopn 2");
					} else if(op_sec == 'o') {
						puts("\tpushr R9\n\tpushr R1\n\tfmod\n\tpopr R1\n\tpopn 2");
					} else {
						error("Unable to resolve floating point operator!\n");
					}
					break;
				case 'a':
					puts("\tpushr R9\n\tpushr R1\n\tfadd\n\tpopr R1\n\tpopn 2");
					break;
				case 's':
					puts("\tpushr R9\n\tpushr R1\n\tfsub\n\tpopr R1\n\tpopn 2");
					break;
				case 'd':
					puts("\tpushr R9\n\tpushr R1\n\tfdiv\n\tpopr R1\n\tpopn 2");
					break;
				default:
					error("Internal Compiler Error! Unable to resolve float operator!\n");
					break;
			}
			puts("\tpopar R9\n");
	  } else if(!strcmp(next_potential_maccess_operator, "conv_f")) {
			match('(');
			do_expression();
			puts("\tpushr R1\n\tfconv\n\tpopr R1\n\tpop");
			match(')');
	  } else if(!strcmp(next_potential_maccess_operator, "conv_i")) {
			match('(');
			do_expression();
			puts("\tpushr R1\n\ticonv\n\tpopr R1\n\tpop");
			match(')');
	  } else if(!strcmp(next_potential_maccess_operator, "left_shift") || !strcmp(next_potential_maccess_operator, "right_shift")) {
			match('(');
			do_expression();
			printf("\tpushar R9\n\tloadrr R9, R1\n");
			match(',');
			do_expression();
			match(')');
			printf("\tpushr R9\n");
			printf("\tpushr R1\n");
			if(!strcmp(next_potential_maccess_operator, "left_shift")) {
				printf("\tshl\n");
			} else {
				printf("\tshr\n");
			}
			printf("\tpopr R1\n");
			printf("\tpopn 2\n");
	   } else if(!strcmp(next_potential_maccess_operator, "not")||!strcmp(next_potential_maccess_operator, "neg")) {
			match('(');
			do_expression();
			match(')');
			if(!strcmp(next_potential_maccess_operator, "neg")) {
				puts("pushr R1\n\tneg\n\tpopr R1\n\tpop");
			} else {
				puts("pushr R1\n\tnot\n\tpopr R1\n\tpop");
			}
	   } else if(!strcmp(next_potential_maccess_operator, "sizeof")) {
		 match('(');
		 eat_blanks();
		 char* struct_name = return_next_tok();
		 match(')');
		 unsigned int size = return_sizeof_struct(struct_name);
		 printf("\tloadr R1, %u\n", size);
	   } else if(!strcmp(next_potential_maccess_operator, "alloc")) {
		  do_alloc();
	   }
	   else {
			printf ("\tloadrm dword, R1, v%s\n", next_potential_maccess_operator);
	  }
}
/*!
 * Called when the compiler expects a statement
 * */
static void
do_statement ()
{
  eat_blanks();
  if (look == ':') {
	get_char();
	if(current_function_name!=NULL) {
		printf("%s.l%s:\n", current_function_name, return_next_tok());
	} else {
		error("label declaration not present in function\n");
	}
  }
  if (look == '@') {
	get_char();
	do_function_call();
  }
  switch (get_keyword ())
    {
    case T_PRINT:  do_print ();  break;
    case T_IF:     do_if ();     break;
    case T_GOTO:   do_goto ();   break;
    case T_INPUT:  do_input ();  break;
    case T_LET:    do_let ();    break;
    case T_GOSUB:  do_gosub ();  break;
    case T_RETURN: do_return (); break;
    case T_REM:    do_rem ();    break;
    case T_END:    do_end ();    break;
    case T_ASM:    do_asm(); break;
    case T_WHILE:  do_while(); break;
    case T_ENDW:   do_endw(); break;
    case T_FUNC:   do_func(); break;
    case T_ENDF:   do_endf(); break;
    case T_STRUCT: do_struct(); break;
    case T_GLOBAL: do_global(); break;
    default:       assert (0);   break;
    }
}
static void
do_global()
{
  eat_blanks();
  add_variable(return_next_tok());
}
static void 
do_struct()
{
  eat_blanks();
  char* new_struct_name = return_next_tok();
  match('(');
  unsigned int struct_members = get_num();
  match(')');
  if(struct_members==0) {
    error("number of struct members is 0\n");
  }
  eat_blanks();
  match('\n');
  line++;
  AddStruct(new_struct_name, struct_members);
  unsigned int Index = 0;
  char * member_type = NULL;
  char* member_name = NULL;
  while(true) {
    member_type = return_next_tok();
    if(!strcmp(member_type, "int8")) {
      eat_blanks();
      member_name = return_next_tok();
      match('\n');
      AddStructMember(member_name, RX_INT8, Index);
      Index++;
      line++;
    } else if(!strcmp(member_type, "int16")) {
      eat_blanks();
      member_name = return_next_tok();
      match('\n');
      AddStructMember(member_name, RX_INT16, Index);
      Index++;
      line++;
    } else if(!strcmp(member_type, "int32")) {
      eat_blanks();
      member_name = return_next_tok();
      match('\n');
      line++;
      AddStructMember(member_name, RX_INT32, Index);
      Index++;
    } else if(!strcmp(member_type, "ends")) {
      if(Index < struct_members) {
	error("Lesser members in struct, than declared!\n");
      }
      break;
    } else {
      error("unknown keyword within struct: %s\n", member_type);
    }
  }
}
/*! 
 * Called when a compiler sees a 'print' keyword.
 * */
static void
do_print ()
{
  use_print_n = 1;

  do_print_item ();

  for (;;)
    {
      if (look == ',')
	{
	  match (',');
	  use_print_t = 1;
	  puts ("\tcall print_t");
	}
      else if (look == ';')
	{
	  match (';');
	}
      else
	{
	  break;
	}

      do_print_item ();
    }

  puts ("\tcall print_n");
}
/*!
 * The actual code that does the generation for the 'print' keyword.
 * */
static void
do_print_item ()
{
  if (look == '"')
    {
      use_print_s = 1;
      do_string ();
      puts ("\tcall print_s");
    }
  else if (look == '$') {
	    get_char();
		use_print_s = 1;
		char* name = return_next_int_name();
		printf ("\tpushr R1\n\tloadrm dword, R1, v%s\n", name);
		printf ("\tcall print_s\n\tpopr R1\n");
  }
  else if(look == '%') {
	get_char();
	do_expression();
	puts("\tcall print_f");
  }
  else
    {
      use_print_i = 1;
      do_expression ();
      puts ("\tcall print_i");
    }
}
/*!
 * Takes one character from terminal and puts it in a variable
 * */
static void
do_input ()
{
  use_input_i = 1;
  do
    printf ("\tcall input_i\n\tpushr R0\n\tpushr R1\n\tloadr R0, v%s\n\tstosd\n\tpopr R1\n\tpopr R0\n", return_next_int_name ());
  while (look == ',');
}
/*!
 * Allocates amount of memory defined by an expression, and sets it to a variable.
 * */
static void
do_alloc()
{
  eat_blanks();
  match('(');
  do_expression();
  match(')');
  printf ("\tcall alloc_n\n");
}
/*!
 * Inline assembler support
 * */
static void 
do_asm() {
  char* asmcode = return_str();
  printf("\t%s\n", asmcode);
}
/*!
 * Code for the if keyword
 * */
static void
do_if ()
{
  int op;
  match('(');
  do_expression ();
  puts ("\tloadrr R4, R1");

  if (look == '=')
    {
      match ('=');
      if (look == '<')
	{
	  match ('<');
	  op = O_LESS_OR_EQUAL;
	}
      else if (look == '>')
	{
	  match ('>');
	  op = O_MORE_OR_EQUAL;
	}
      else
	{
	  op = O_EQUAL;
	}
    }
  else if (look == '<')
    {
      match ('<');
      if (look == '=')
	{
	  match ('=');
	  op = O_LESS_OR_EQUAL;
	}
      else
	{
	  op = O_LESS;
	}
    }
  else if(look == '!') {
	match('!');
	if(look == '=') {
		match('=');
		op = O_NOT_EQUAL;
	} else {
		error("Unknown operator!\n");
	}
  }
  else if (look == '>')
    {
      match ('>');
      if (look == '=')
	{
	  match ('=');
	  op = O_MORE_OR_EQUAL;
	}
      else if (look == '<')
	{
	  match ('<');
	  op = O_NOT_EQUAL;
	}
      else
	{
	  op = O_MORE;
	}
    }
  else
    {
      error ("expected =, <>, ><, <=, =<, >= or => got '%c'", look);
    }

  do_expression ();

  fputs ("\tcmpr R4, R1\n\tj", stdout);

  /* note inversal of operators */
  if (op == O_EQUAL)
    fputs ("ne", stdout);
  else if (op == O_NOT_EQUAL)
    fputs ("e", stdout);
  else if (op == O_LESS)
    fputs ("ge", stdout);
  else if (op == O_MORE)
    fputs ("le", stdout);
  else if (op == O_LESS_OR_EQUAL)
    fputs ("g", stdout);
  else if (op == O_MORE_OR_EQUAL)
    fputs ("l", stdout);

  printf (" i%i\n", line);
  match(')');
  do_statement ();

  printf ("i%i:\n", line);
}
/*!
 * While keyword
 * */
/*!
 * A = 0
 * WHILE A < 5 ---> while0 :
 * 					cmpr A, 5
 * 	A = A + 1		jge endw0
 * ENDW				mov A, A+1
 * 					jmp while0
 * 					endw0:
 * */				
static void 
do_while()
{
  int op;
  printf("while%u:\n", whilelines);
  while_stack[currentwhile] = whilelines;
  match('(');
  do_expression ();
  puts ("\tloadrr R4, R1");

  if (look == '=')
    {
      match ('=');
      if (look == '<')
	{
	  match ('<');
	  op = O_LESS_OR_EQUAL;
	}
      else if (look == '>')
	{
	  match ('>');
	  op = O_MORE_OR_EQUAL;
	}
      else
	{
	  op = O_EQUAL;
	}
    }
  else if (look == '<')
    {
      match ('<');
      if (look == '=')
	{
	  match ('=');
	  op = O_LESS_OR_EQUAL;
	}
      else
	{
	  op = O_LESS;
	}
    }
  else if(look == '!') {
	match('!');
	if(look == '=') {
		match('=');
		op = O_NOT_EQUAL;
	} else {
		error("Unknown operator!\n");
	}
  }
  else if (look == '>')
    {
      match ('>');
      if (look == '=')
	{
	  match ('=');
	  op = O_MORE_OR_EQUAL;
	}
      else if (look == '<')
	{
	  match ('<');
	  op = O_NOT_EQUAL;
	}
      else
	{
	  op = O_MORE;
	}
    }
  else
    {
      error ("expected =, !=, ><, <=, =<, >= or => got '%c'", look);
    }

  do_expression ();

  fputs ("\tcmpr R4, R1\n\tj", stdout);

  /* note inversal of operators */
  if (op == O_EQUAL)
    fputs ("ne", stdout);
  else if (op == O_NOT_EQUAL)
    fputs ("e", stdout);
  else if (op == O_LESS)
    fputs ("ge", stdout);
  else if (op == O_MORE)
    fputs ("le", stdout);
  else if (op == O_LESS_OR_EQUAL)
    fputs ("g", stdout);
  else if (op == O_MORE_OR_EQUAL)
    fputs ("l", stdout);
  printf(" endwhile%u\n", whilelines);
  match(')');
  whilelines++;
  currentwhile--;
}
/*!
 * End of a while statement.
 * */
static void 
do_endw()
{
	int currentwhile_now = while_stack[currentwhile+1];
	printf("\tjmp while%u\n", currentwhile_now);
	printf("endwhile%u:\n", currentwhile_now);
	currentwhile++;
}
/*!
 * Does a goto (jump)
 * */
static void
do_goto ()
{
  if(isalpha(look) && current_function_name!=NULL) {
	printf("\tjmp %s.l%s\n", current_function_name, return_next_tok());
  } else {
	error("unknown goto.\n");
  }
}
/*!
 * Does a gosub (call)
 * */
static void
do_gosub ()
{
  if(isalpha(look) && current_function_name != NULL) {
	printf("\tcall %s.l%s\n", current_function_name, return_next_tok());
  } else {
	error("unknown gosub.\n");
  }
}
/*!
 * Does a return
 * */
static void
do_return ()
{
  eat_blanks();
  do_expression();
  puts("\tloadrr R7, R1");
  puts ("\tret");
}
/*!
 * Sets a variable [or the address to which it points to] to a value defined by an expression
 * passed to it.
 * */
static void
do_let ()
{
	eat_blanks();
	if(look=='<') {
	  get_char();
	  RX_STRUCT_TYPE expr_type = do_typecast(1);
	  puts("\tpushar R0");
	  eat_blanks();
	  match('=');
	  do_expression();
	  puts("\tpopar R0");
	  switch(expr_type) {
	    case RX_INT8:
		puts("stosb");
		break;
	    case RX_INT16:
		puts("stosw");
		break;
	    case RX_INT32:
		puts("stosd");
		break;
	    default:
		error("INTERNAL COMPILER ERROR: Unable to resolve structure type!\n");
		break;
	  }
	  return;
	}
	char* name = return_next_int_name_and_add ();
	if(!strcmp(name, "int32_ptr") || (!strcmp(name, "int8_ptr")) || !(strcmp(name, "int16_ptr"))) {
		char maccess_operator = name[3];
		match('(');
		do_expression();
		match(')');
		printf("\tloadrr R5, R1\n");
		match('=');
		do_expression();
		switch(maccess_operator) {
		case '1':
			printf("\tpushr R0\n\tloadrr R0, R5\n\tstosw\n\tpopr R0\n");
			break;
		case '3':
			printf("\tpushr R0\n\tloadrr R0, R5\n\tstosd\n\tpopr R0\n");
			break;
		case '8':
			printf("\tpushr R0\n\tloadrr R0, R5\n\tstosb\n\tpopr R0\n");
			break;
		default:
			puts("; Warning: Internal compiler error. Unable to find maccess operator!");
			puts ("\tpushr R0\n\tloadrr R0, R5\n\tstosb\n\tpopr R0\n");
			break;
		}
	  }
	else {
		match('=');
		do_expression ();
		printf ("\tloadr R0, v%s\n\tstosd\n", name);
	}
 }
/*!
 * Ignores everything till it receives a newline character
 */
static void
do_rem ()
{
  while (look != '\n')
    get_char ();
}
/*!
 * Quits the program
 * */
static void
do_end ()
{
  puts ("\tjmp _exit");
}
static void 
do_func() {
	eat_blanks();
	char* function_name = return_next_tok();
	match('(');
	unsigned int arguments = get_num();
	match(')');
	add_to_function_table(function_name, arguments);
	printf("function %s\n", function_name);
	if(!strcmp(function_name, "main")) {
	  //! add some codez to mainz.
	  printf("\tcall __internal_init\n");
	}
	current_function_name = function_name;
}
static void
do_endf() {
	if(current_function_name!=NULL) {
		printf("endfunction %s\n", current_function_name);
		current_function_name = NULL;
	} else {
		error("endf before any function declaration!\n");
	}
}
static void
do_function_call() {
	stuff_pushed_to_stack = 0;
	char* function_name = return_next_tok();
	function_type* type = return_function_type_if_function_exists(function_name);
	if(type==NULL) {
		error("function: %s not declared!\n", function_name);
	}
	match('(');
	unsigned int i = 0;
	if(type->number_of_arguments==0) {
		//! No args need to passed.
		match(')');
	}
	else {
		while(i<type->number_of_arguments) {
			do_expression();
			if(i<type->number_of_arguments-1) {
				match(',');
			} else {
				match(')');
			}
			puts("\tpushr R1\n");
			stuff_pushed_to_stack++;
			i++;
		}
	}
	//! Save util registers
	printf("; Save utility registers\n");
	printf("\tpushr R4\n\tpushr R5\n\tpushr R9\n\tpushr R10\n");
	printf("\tcall %s\n", function_name);
	printf("\tpopr R10\n\tpopr R9\n\tpopr R5\n\tpopr R4\n");
	printf("\tpopn %u\n", type->number_of_arguments);
	printf("\tloadrr R1, R7\n");
	stuff_pushed_to_stack = 0;
	
}
static RX_STRUCT_TYPE
do_typecast(int signal) {
  RX_STRUCT_TYPE mytype;
  eat_blanks();
  char* type_cast_type = return_next_tok();
  if(!strcmp(type_cast_type, "global")) {
    //! this is a global typecast.
    eat_blanks();
    match('>');
    eat_blanks();
    char* var_name = return_next_tok();
    add_variable(var_name);
    printf("\tloadr R0, v%s\n", var_name);
    if(signal == 0) {
      printf("\tlodsd\n");
    }
    return RX_INT32;
  } else if(strcmp(type_cast_type, "struct")) {
    error("unknown typecast! %s\n", type_cast_type);
  }
  eat_blanks();
  char* get_type_dest = return_next_tok();
  if(return_struct_type_if_struct_exists(get_type_dest)==NULL) {
    error("struct %s undefined\n", get_type_dest);
  }
  eat_blanks();
  match('>');
  eat_blanks();
  do_expression();
  eat_blanks();
  if(look=='.') {
    get_char();
    char* struct_submember = return_next_tok();
    mytype = return_type_if_struct_member_exists(get_type_dest, struct_submember);
    unsigned int offset = return_member_offset(get_type_dest, struct_submember);
    puts("\tloadrr R0, R1");
    printf("\taddr R0, %u\n", offset);
    if(signal == 0) {
	if(mytype == RX_INT8) {
	  puts("\tlodsb");
	 } else if(mytype == RX_INT16) {
	    puts("\tlodsw");
	 } else {
	    puts("\tlodsd");
	 }
      } else {
	return mytype;
      }
    }
    else {
      ungetc(look, stdin);
    }
    return mytype;
}
/* vim:set ts=8 sts=2 sw=2 noet: */
