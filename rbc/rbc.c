#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#define DEFAULT_INITIAL_AND_GROWTH_SIZE 16
typedef enum {
	StringLiteral,
	IntegerLiteral,
	Identifier,
	Operator,
	Keyword
} TokenType;
typedef struct {
	char* TokenString;
	TokenType Type;
} Token;
typedef struct {
	Token* Tokens;
	unsigned int TotalTokens;
} TokenList;

TokenList* TokenizeString(char* Source);
void AddToken(TokenList* List, int CurrentToken, char* TokenToAdd);
bool CheckIfStringIsEmpty(char* Str);

int main(int argc, char** argv){
	char* MyStr = "PRINT \"Hello, World!\"\nQUIT";
	TokenList* newlist = TokenizeString(MyStr);
	for(int i = 0; i < newlist->TotalTokens; i++){
		printf("token: %s\n", newlist->Tokens[i].TokenString);
	}
	return 0;
}

TokenList* TokenizeString(char* Source){
	unsigned int CurrentToken = 0; char* CurrentTokenStr = calloc(sizeof(char), DEFAULT_INITIAL_AND_GROWTH_SIZE); int CurrentTokenSz = DEFAULT_INITIAL_AND_GROWTH_SIZE; int CurrentOffset = 0;
	TokenList* InitialList = malloc(sizeof(TokenList));
	InitialList->Tokens = malloc(DEFAULT_INITIAL_AND_GROWTH_SIZE * sizeof(Token));
	InitialList->TotalTokens = DEFAULT_INITIAL_AND_GROWTH_SIZE;
	bool InsideString = false;
	for(unsigned int i = 0; i <= strlen(Source); i++){
		if(Source[i] == ' ' || Source[i] == '\n' || Source[i] == '\t' || Source[i] == '\r' || Source[i] == 0){
			if(InsideString == false){
				bool IsStringEmpty = CheckIfStringIsEmpty(CurrentTokenStr);
				if(IsStringEmpty){
					AddToken(InitialList, CurrentToken, CurrentTokenStr);
					if(Source[i] == 0) { break; }
					memset(CurrentTokenStr, 0, CurrentTokenSz);
					CurrentOffset = 0;
					CurrentToken++;
				}
			}
		} else if(Source[i] == '\"'){
			InsideString = !InsideString;
			if(InsideString == false){
				AddToken(InitialList, CurrentToken, CurrentTokenStr);
				memset(CurrentTokenStr, 0, CurrentTokenSz);
				CurrentOffset = 0;
				CurrentToken++;
			}
		} else {
			if(CurrentOffset > CurrentTokenSz-1){
				char* NewStr = realloc(CurrentTokenStr, CurrentTokenSz + DEFAULT_INITIAL_AND_GROWTH_SIZE);
				if(!NewStr){
					fprintf(stderr, "Failed to reallocate current token buffer.\n");
					return NULL;
				}
				CurrentTokenStr = NewStr;
				CurrentTokenSz += DEFAULT_INITIAL_AND_GROWTH_SIZE;
			}
			CurrentTokenStr[CurrentOffset] = Source[i];
			CurrentOffset++;
		}
	}
	return InitialList;
}
void ScanTokenList(TokenList* List){
	
}
void AddToken(TokenList* List, int CurrentToken, char* TokenToAdd){
	if(CurrentToken > List->TotalTokens-1){
				Token* NewList = realloc(List->Tokens, (List->TotalTokens + DEFAULT_INITIAL_AND_GROWTH_SIZE) * sizeof(Token));
				if(!NewList){
					fprintf(stderr, "Failed to reallocate token list buffer\n");
					return;
				}
				List->Tokens = NewList;
				List->TotalTokens += DEFAULT_INITIAL_AND_GROWTH_SIZE;
			}
	List->Tokens[CurrentToken].TokenString = strdup(TokenToAdd);
}
bool CheckIfStringIsEmpty(char* Str){
	for(unsigned int i = 0; i < strlen(Str); i++){
		if(Str[i] != '\0' && Str[i] != '\t' && Str[i] && '\r' && Str[i] != '\n'){
			return true;
		}
	}
	return false;
}
