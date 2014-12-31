using System;
using System.Collections.Generic;
namespace cc
{
	public class Parser
	{
		public static int CurrentToken;
		public static List<Scanner.token_t> TokenList;
		public static void ParserInit(List<Scanner.token_t> Tokens){
			TokenList = Tokens;
			CurrentToken = 0;
		}
		public static Scanner.token_t ReadToken(){
			Scanner.token_t temp = TokenList [CurrentToken];
			CurrentToken++;
			return temp;
		}
		public static bool OpenParenThesis() {
			if (TokenList [CurrentToken].token_string == "(" && TokenList[CurrentToken].token_id == Scanner._BLOCKINDICATOR) {
				return true;
			} else {
				return false;
			}
		}
		public static bool CheckSemiColon(){
			if (TokenList [CurrentToken].token_string == ";" && TokenList [CurrentToken].token_id == Scanner._OPERATOR) {
				return true;
			} else {
				return false;
			}
		}
		public static bool CloseParenThesis(){
			if (TokenList [CurrentToken].token_string == ")" && TokenList [CurrentToken].token_id == Scanner._BLOCKINDICATOR) {
				return true;
			} else {
				return false;
			}
		}
		public static string ReturnParserString(int off){
			return TokenList [off].token_string;
		}
	}
}

