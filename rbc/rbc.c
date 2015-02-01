#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
//! @DEFAULT_INITIAL_AND_GROWTH_SIZE : Default size of a list, and it's incremental size.
#define DEFAULT_INITIAL_AND_GROWTH_SIZE 16
/*!
 * @TokenType : Tells what class a token belongs to.
 */
typedef enum {
	NULL_TYPE = 0,
	StringLiteral,
	IntegerLiteral,
	InternalFunction,
	Identifier,
	Operator,
	EndStatement,
	EndLabelDeclaration,
	LanguageType,
	Keyword
} TokenType;
/*!
 * @Token : Defines a token structure.
 */
typedef struct {
	char* TokenString;
	TokenType Type;
	int Line;
} Token;
/*!
 * @TokenList : A list containing an n number of tokens. The number of tokens it contains it store in TokenList.TotalTokens
 */
typedef struct {
	Token* Tokens;
	unsigned int TotalTokens;
	unsigned int CurrentToken;
} TokenList;
/*!
 * @ExpressionType : Defines what can contain in an expression
 */
typedef enum {
	ENULL_TYPE = 0,
	EOpenParenthesis,
	ECloseParenthesis,
	EIntegerLiteral,
	EVariable,
	EOperator,
} ExpressionType;
/*!
 * @ExpressionUnit : Defines the smalles unit of an expression
 */
typedef struct {
	ExpressionType Type;
	char* UnitName;
	int Value;
} ExpressionUnit;
/*!
 * @ExpressionStack : A stack containing smaller expression units.
 */
typedef struct {
	ExpressionUnit* Units;
	unsigned int TopOfStack;
	unsigned int TotalNumberOfUnits;
} ExpressionStack;

/*!
 * @Function Prototypes
 */
TokenList* TokenizeString(char* Source);

void AddToken(TokenList* List, unsigned int CurrentToken, char* TokenToAdd, TokenType Type, unsigned int LineNumber);

bool CheckIfStringIsEmpty(char* Str);

void PrintType(Token Tok);

void FreeTokenList(TokenList* List);

TokenType GetType(char* Tok);

int PushtoExpressionStack(ExpressionStack* Stack, char* Name, ExpressionType Type, unsigned int Value);

ExpressionUnit PopFromExpressionStack(ExpressionStack* Stack);

/*!
 * @main
 * @param int argc, char** argv
 * @return int
 * @Description
 * @Standard C main function.
 */
int main(int argc, char** argv){
	char* MyStr = "1+2";
	//!char* MyStr = "PRINT \"Hello, World!\";\nQUIT; A=5*12&36; 5122; IF A <> 512 THEN GOTO 500;";
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
//! @CheckIfStringIsEmpty
//! @param char* Str : Pointer to a NULL terminated string
//! @return bool : Boolean value
//! @Description:
//! @Takes a NULL terminated string, and checks if it contains only white spaces, 
//! @which are either '\0' [NULL Terminator], '\t' [Tab character], '\r' [Carriage return], '\n' [Newline character], ' ' [ASCII space character].
//! @Return value is a boolean, true if the string only contained whitespaces, false if the string contains
//! @non-whitespace characters. 
bool CheckIfStringIsEmpty(char* Str){
	for(unsigned int i = 0; i < strlen(Str); i++){
		if(Str[i] != '\0' && Str[i] != '\t' && Str[i] && '\r' && Str[i] != '\n' && Str[i] != ' '){
			return true;
		}
	}
	return false;
}
//! @CheckIfStringIsInteger
//! @param char* Str : Pointer to a NULL terminated string
//! @return bool : Boolean value
//! @Description:
//! @Takes a NULL terminated string, and checks if it contains only ASCII
//! @numbers, '0' to '9', and returns true if yes, false if no.
bool CheckIfStringIsInteger(char *s){
	for(int i = 0; i < strlen(s); i++){
		if(!isdigit(s[i])){
			return false;
		}
	}
	return true;
}
//! @TokenizeString
//! @param char* Source : Pointer to a NULL terminated string
//! @return TokenList* : Pointer to a token list
//! @Description:
//! @Takes in a NULL terminated string as BASIC source code, and then tokenizes them
//! @into a list. The list contains various information about every term which is in the
//! @BASIC source, such as it's description (Identifier, Keyword, etc..), line number, etc.
//! @The tokenizer is also able to separate strings from normal code and add them individually,
//! @without the removal of white spaces, and symbols.
//! @It also parses operators, even without whitespaces, like:
//! @12%6
//! @Would be outputted as: 12 - Integer Literal, % - Operator, 6 - Integer Literal.
//! @The output is a pointer to a TokenList, allocated by the function using malloc(1), it must be 
//! @freed using FreeList(1), on exit or after use.
TokenList* TokenizeString(char* Source){
	unsigned int CurrentToken = 0; char* CurrentTokenStr = calloc(sizeof(char), DEFAULT_INITIAL_AND_GROWTH_SIZE); int CurrentTokenSz = DEFAULT_INITIAL_AND_GROWTH_SIZE; unsigned int CurrentOffset = 0; unsigned int CurrentLineNo = 0;
	TokenList* InitialList = malloc(sizeof(TokenList));
	InitialList->Tokens = malloc(DEFAULT_INITIAL_AND_GROWTH_SIZE * sizeof(Token));
	InitialList->TotalTokens = DEFAULT_INITIAL_AND_GROWTH_SIZE;
	InitialList->CurrentToken = 0;
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
		} else if(InsideString == false && (Source[i] == '*' || Source[i] == '/' || Source[i] == '+' || Source[i] == '-'|| Source[i] == '&' || Source[i] == '^' || Source[i] == '|' || Source[i] == '>' || Source[i] == '<' || Source[i] == '%' || Source[i] == '!' || Source[i] == '=' || Source[i] == ':' || Source[i] == ';')){
				//! Add whatever was there in current token..
				if(CheckIfStringIsEmpty(CurrentTokenStr)) { AddToken(InitialList, CurrentToken, CurrentTokenStr, GetType(CurrentTokenStr), CurrentLineNo); CurrentToken++; }
				//! Now add the operator token..
				char OperatorStr[2];
				OperatorStr[0] = Source[i];
				OperatorStr[1] = '\0';
				if(Source[i] == ':'){
					AddToken(InitialList, CurrentToken, OperatorStr, EndLabelDeclaration, CurrentLineNo);
				} else if (Source[i] == ';') {
					AddToken(InitialList, CurrentToken, OperatorStr, EndStatement, CurrentLineNo);
				} else {
					AddToken(InitialList, CurrentToken, OperatorStr, Operator, CurrentLineNo);
				}
				CurrentToken++;
				memset(CurrentTokenStr, 0, CurrentTokenSz);
				CurrentOffset = 0;
		} else if(Source[i] == '\"'){
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
//! @AddToken
//! @param TokenList*, unsigned int, char*, TokenType, unsigned int
//! @return void
//! @Description:
//! @Takes a TokenList* as an argument, and the current token argument, with a char* as a pointer to the
//! @token string, and it's respective type with it's line number, and then adds the token to the list.
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
//! @FreeTokenList
//! @param TokenList*
//! @return void
//! @Description:
//! @Takes a TokenList* as an argument, and then frees all it's strings and finally the list
//! @itself. The list given MUST be allocated using @TokenizeString
void FreeTokenList(TokenList* List) {
	for(unsigned int i = 0; i < List->TotalTokens; i++) {
		if(List->Tokens[i].TokenString != NULL) {
			free(List->Tokens[i].TokenString);
		}
	}
	free(List->Tokens);
	free(List);
}
//! @GetType
//! @param char* Tok : Pointer to a NULL terminated string.
//! @return TokenType
//! @Description:
//! @Takes in a token string, and returns a token type based on what the string is.
//! @Values that can be expected from the function's return mechanism can be used to build token
//! @token lists.
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
	} else if(strcmp(Tok, "THEN") == 0) {
	  return Keyword;
	} else if(strcmp(Tok, "GOSUB") == 0) {
	  return Keyword;
	} else if(strcmp(Tok, "STRING") == 0){
	  return LanguageType;
	} else if(strcmp(Tok, "INTEGER") == 0){
	  return LanguageType;
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
//! @PrintType
//! @param Token Tok : A token created by @TokenizeString
//! @return void
//! @Description:
//! @Takes in a token, and prints out it's type to stdout.
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
	} else if(Tok.Type == EndStatement){
		printf("Statement End\n");
	} else if(Tok.Type == EndLabelDeclaration){
		printf("Label Declaration\n");
	} else if(Tok.Type == LanguageType) {
		printf("Language Type\n");
	} else {
		printf("Unknown Token\n");
	}
}
/*!
 * @GenerateCode
 * @param TokenList*
 * @return void
 * @Description
 * <none>
 */
void ParseExpression(TokenList* List){
	
}
ExpressionStack* CreateExpressionStack(void) {
	ExpressionStack* NewStack = malloc(sizeof(ExpressionStack));
	NewStack->TotalNumberOfUnits = 0;
	NewStack->TopOfStack = 0;
	NewStack->Units = NULL;
}
/*!
 * @PushtoExpressionStack 
 * @param ExpressionStack*, char*, ExpressionType, unsigned int
 * @return int
 * @Description
 * @Pushes an expression unit to an expression stack, with appropriate properties,
 * @returns 0 on success and -1 on error.
 */
int PushtoExpressionStack(ExpressionStack* Stack, char* Name, ExpressionType Type, unsigned int Value){
	if(Stack->TopOfStack == 0){
		Stack->Units = malloc(DEFAULT_INITIAL_AND_GROWTH_SIZE * sizeof(ExpressionUnit));
		Stack->TotalNumberOfUnits += DEFAULT_INITIAL_AND_GROWTH_SIZE;
	} else if(Stack->TopOfStack >= Stack->TotalNumberOfUnits) {
		ExpressionUnit* newUnits = realloc(Stack->Units, Stack->TotalNumberOfUnits + DEFAULT_INITIAL_AND_GROWTH_SIZE);
		if(!newUnits){
			fprintf(stderr, "Unable to reallocate expression buffer stack.\n");
			return -1;
		}
		Stack->Units = newUnits;
	}
	Stack->Units[Stack->TopOfStack].Type = Type;
	Stack->Units[Stack->TopOfStack].Value = Value;
	Stack->Units[Stack->TopOfStack].UnitName = strdup(Name);
	Stack->TopOfStack++;
	return 0;
}
/*!
 * @PopFromExpressionStack
 * @param ExpressionStack*
 * @return ExpressionUnit
 * @Description
 * @Returns an ExpressionUnit from the top of an expressions' stack and then 
 * @decreses the top of stack by 1.
 * @NOTE: Returns a NULL stack if top of stack is zero or greater than the stack
 * @limit. 
 */
ExpressionUnit PopFromExpressionStack(ExpressionStack* Stack) {
	ExpressionUnit NullUnit;
	if(Stack->TopOfStack >= Stack->TotalNumberOfUnits || Stack->TopOfStack == 0){
		return NullUnit;
	} else {
		Stack->TopOfStack--;
		return Stack->Units[Stack->TopOfStack+1];
	}
}
