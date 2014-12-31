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
			if (CurrentToken >= TokenList.Count) {
				Console.WriteLine ("Read all tokens. Parser Error!");

			}
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
		public static List<string> ReadUntilEndParenthesis(){
			Stack<string> ParenthesisStack = new Stack<string>();
			List<string> ArgList = new List<string> ();
			string mystring = "";
			while (mystring != null) {
				mystring = ReadToken ().token_string;
				if (mystring == "(") {
					ParenthesisStack.Push ("(");
					ArgList.Add ("(");
				} else if (mystring == ")") {
					ParenthesisStack.Pop ();
					ArgList.Add (")");
					if (ParenthesisStack.Count == 0) {
						return ArgList;
					}
				} else if (mystring == "{" || mystring == null || mystring == ";") {
					return ArgList;
				} else {
					ArgList.Add (mystring);
				}
			}
			return ArgList;
		}
		public static string ReturnParserString(int off){
			return TokenList [off].token_string;
		}
		public static List<string> ReadUntilSemiColon(){
			List<string> stuff = new List<string> ();
			string mystring = "";
			while (mystring != ";" || mystring != null) {
				mystring = ReadToken ().token_string;
				if (String.IsNullOrWhiteSpace (mystring) == true || mystring == ";") {
					return stuff;
				} else {
					stuff.Add (mystring);
				}
			}
			return stuff;
		}
	}
}

