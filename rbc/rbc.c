#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define DEFAULT_INITIAL_AND_GROWTH_SIZE 16
typedef enum {
	NULL_TYPE = 0,
	StringLiteral,
	IntegerLiteral,
	InternalFunction,
	Identifier,
	Operator,
	Keyword
} TokenType;
typedef struct {
	char* TokenString;
	TokenType Type;
	int Line;
} Token;
typedef struct {
	Token* Tokens;
	unsigned int TotalTokens;
} TokenList;

TokenList* TokenizeString(char* Source);
void AddToken(TokenList* List, unsigned int CurrentToken, char* TokenToAdd, TokenType Type, unsigned int LineNumber);
bool CheckIfStringIsEmpty(char* Str);
void PrintType(Token Tok);
void FreeTokenList(TokenList* List);

TokenType GetType(char* Tok);

int main(int argc, char** argv){
	char* MyStr = "PRINT \"Hello, World!\"\nQUIT A=5*12&36 5122 IF A <> 512 THEN GOTO 500";
	printf("Lexer Input: %s\n", MyStr);
	TokenList* newlist = NULL;
	newlist = TokenizeString(MyStr);
	for(int i = 0; i < newlist->TotalTokens; i++){
	  if(newlist->Tokens[i].TokenString != NULL){
		printf("token: %s, line : %u, Type: ", newlist->Tokens[i].TokenString, newlist->Tokens[i].Line);
		PrintType(newlist->Tokens[i]);
	  }
	}
	FreeTokenList(newlist);
	return 0;
}

TokenList* TokenizeString(char* Source){
	unsigned int CurrentToken = 0; char* CurrentTokenStr = calloc(sizeof(char), DEFAULT_INITIAL_AND_GROWTH_SIZE); int CurrentTokenSz = DEFAULT_INITIAL_AND_GROWTH_SIZE; unsigned int CurrentOffset = 0; unsigned int CurrentLineNo = 0;
	TokenList* InitialList = malloc(sizeof(TokenList));
	InitialList->Tokens = malloc(DEFAULT_INITIAL_AND_GROWTH_SIZE * sizeof(Token));
	InitialList->TotalTokens = DEFAULT_INITIAL_AND_GROWTH_SIZE;
	bool InsideString = false;
	for(unsigned int i = 0; i <= strlen(Source); i++){
		if(Source[i] == '\n'){
			CurrentLineNo++;		
		}
		if(InsideString == false && (Source[i] == ' ' || Source[i] == '\n' || Source[i] == '\t' || Source[i] == '\r' || Source[i] == 0)){
				bool IsStringEmpty = CheckIfStringIsEmpty(CurrentTokenStr);
				if(IsStringEmpty){
					TokenType MyType = GetType(CurrentTokenStr);
					AddToken(InitialList, CurrentToken, CurrentTokenStr, MyType, CurrentLineNo);
					if(Source[i] == 0) { break; }
					memset(CurrentTokenStr, 0, CurrentTokenSz);
					CurrentOffset = 0;
					CurrentToken++;
				}
		} else if(Source[i] == '*' || Source[i] == '/' || Source[i] == '+' || Source[i] == '-'|| Source[i] == '&' || Source[i] == '^' || Source[i] == '|' || Source[i] == '>' || Source[i] == '<' || Source[i] == '%' || Source[i] == '!' || Source[i] == '='){
			if(InsideString == false){
				//! Add whatever was there in current token..
				if(CheckIfStringIsEmpty(CurrentTokenStr)) { AddToken(InitialList, CurrentToken, CurrentTokenStr, GetType(CurrentTokenStr), CurrentLineNo); CurrentToken++; }
				//! Now add the operator token..
				char OperatorStr[2];
				OperatorStr[0] = Source[i];
				OperatorStr[1] = '\0';
				AddToken(InitialList, CurrentToken, OperatorStr, Operator, CurrentLineNo);
				CurrentToken++;
				memset(CurrentTokenStr, 0, CurrentTokenSz);
				CurrentOffset = 0;
			}
		}else if(Source[i] == '\"'){
			InsideString = !InsideString;
			if(InsideString == false){
				AddToken(InitialList, CurrentToken, CurrentTokenStr, StringLiteral, CurrentLineNo);
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
	free(CurrentTokenStr);
	return InitialList;
}
void AddToken(TokenList* List, unsigned int CurrentToken, char* TokenToAdd, TokenType Type, unsigned int LineNumber){
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
	List->Tokens[CurrentToken].Type = Type;
	List->Tokens[CurrentToken].Line = LineNumber;
}
bool CheckIfStringIsEmpty(char* Str){
	for(unsigned int i = 0; i < strlen(Str); i++){
		if(Str[i] != '\0' && Str[i] != '\t' && Str[i] && '\r' && Str[i] != '\n'){
			return true;
		}
	}
	return false;
}
void PrintType(Token Tok){
	if(Tok.Type == InternalFunction){
		printf("Internal Function\n");
	} else if(Tok.Type == Identifier) {
		printf("Identifier\n");
	} else if(Tok.Type == StringLiteral){
		printf("String Literal\n");
	} else if(Tok.Type == IntegerLiteral){
		printf("Integer Literal\n");
	} else if(Tok.Type == Operator){
		printf("Operator\n");
	} else if(Tok.Type == Keyword){
		printf("Language Keyword\n");
	} else {
		printf("Unknown Token\n");
	}
}
bool CheckIfStringIsInteger(char *s){
	for(int i = 0; i < strlen(s); i++){
		if(!isdigit(s[i])){
			return false;
		}
	}
	return true;
}
TokenType GetType(char* Tok){
	if(strcmp(Tok, "PRINT") == 0){
		return InternalFunction;
	} else if(strcmp(Tok, "INPUT") == 0){
		return InternalFunction;
	} else if(strcmp(Tok, "QUIT") == 0){
		return InternalFunction;
	} else if(strcmp(Tok, "IF") == 0){
		return Keyword;
	} else if(strcmp(Tok, "WHILE") == 0){
		return Keyword;
	} else if(strcmp(Tok, "GOTO") == 0){
		return Keyword;
	} else if(strcmp(Tok, "ASM") == 0){
		return Keyword;
	} else {
		//! Check if it's an integer
		bool IsInt = CheckIfStringIsInteger(Tok);
		if(IsInt){
			return IntegerLiteral;
		} else {
			return Identifier;
		}
	}
}
void FreeTokenList(TokenList* List) {
	for(unsigned int i = 0; i < List->TotalTokens; i++) {
		if(List->Tokens[i].TokenString != NULL) {
			free(List->Tokens[i].TokenString);
		}		
	}
	free(List->Tokens);
	free(List);
}
