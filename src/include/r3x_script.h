#ifndef R3X_SCRIPT_H
#define R3X_SCRIPT_H
#include <system.h>

typedef struct {
	char* ExeName;
	char* FontName;
	char* Args;
	uint64_t StackSize;
	float Frequency;
	float FontScale;
	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
	unsigned int FontHeight;
	unsigned int FontWidth;
} ScriptState;

void parse_script(char*, ScriptState*);
#endif
