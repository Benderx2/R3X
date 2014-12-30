/** cgr3x.c - Smaller C code generator for the R3X architecture **/
#include "smlrc_r3x.h"
STATIC
void GenInit(void) { 
	SizeOfWord = REX_DEFAULT_WORD_SIZE;
	OutputFormat = FormatFlat;
	CodeHeader = "\t.";
}
