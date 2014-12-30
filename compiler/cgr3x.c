/** cgr3x.c - Smaller C code generator for the R3X architecture **/
#include "smlrc_r3x.h"
STATIC
void GenInit(void) { 
	SizeOfWord = REX_DEFAULT_WORD_SIZE;
	OutputFormat = FormatFlat;
	CodeHeader = "\t.text {";
	FileHeader = "\tinclude 'libR3X/libR3X.inc";
	UseLeadingUnderscores = 0;
}
STATIC 
int GenInitParams(int argc, char** argv. int* idx) {
	(void)argc;
	/** Check if we're compiling a dynamic library **/
	if(!strcmp(argv[*idx], "-shared")) {
		printf("shared option is not currently supported. Sorry!\n");
		return -1;
		// TODO: Add support for dynamic libraries
	}
	return 0;
}
STATIC
void GenInitFinalize(void)
{
  // finalization of initialization of target-specific code generator
  printf2("end");
}
STATIC
void GenStartCommentLine(void)
{
	printf2(";");
}
STATIC
void GenWordAlignment(void)
{
  printf2("\talign __REX_WORD\n");
}
STATIC
void GenLabel(char* Label, int Static)
{
    printf2("%s:\n", Label);
}
STATIC
void GenPrintLabel(char* Label)
{
  {
    if (isdigit(*Label))
      printf2("$L%s", Label);
    else
      printf2("%s", Label);
  }
}
void GenNumLabel(int Label)
{
  printf2("$L%d:\n", Label);
}

STATIC
void GenPrintNumLabel(int label)
{
  printf2("$L%d", label);
}

STATIC
void GenZeroData(unsigned Size)
{
  printf2("\ttimes\t%u db 0\n", truncUint(Size)); // or ".fill size"
}
STATIC
void GenIntData(int Size, int Val)
{
  Val = truncInt(Val);
  if (Size == 1)
    printf2("\tdb\t%d\n", Val);
  else if (Size == 2)
    printf2("\tdw\t%d\n", Val);
  else if (Size == 4)
    printf2("\tdd\t%d\n", Val);
}
STATIC
void GenStartAsciiString(void)
{
  printf2("\tdb\t");
}
