using System;
using System.Collections.Generic;
namespace cc
{
	public class Parser
	{
		public static bool IsEnd = false;
		public static int CurrentToken;
		public static List<Scanner.token_t> GlobalTokenList;
		public static void ParserInit(List<Scanner.token_t> TokenList){
			GlobalTokenList = TokenList;
			CurrentToken = 0;
			CodeGen.CodeGenInit ("output.txt");
		}
		public static void ParseExpression(){
			Scanner.token_t mytok;
			CodeGen.TextSection ();
				while (IsEnd == false) {
				mytok = ReadToken ();
				if (mytok.token_id != Scanner._OPERATOR && mytok.token_id != Scanner._LITERAL_NUM && mytok.token_id != Scanner._SEMICOLON && mytok.token_id != Scanner._BLOCKINDICATOR) {
					throw new Exception (string.Format ("Unknown Character!"));
				}
				ProcessToken (mytok);
			}
			CodeGen.EndSection ();
			CodeGen.GenFinalize ();
		}
		public static void ProcessToken(Scanner.token_t Token){
			switch (Token.token_id) {
			case Scanner._LITERAL_NUM:
			case Scanner._BLOCKINDICATOR:
			case Scanner._OPERATOR:
				ProcessOperator (Token);
				break;
			case Scanner._SEMICOLON:
				IsEnd = true;
				break;
			}
		}
		public static void ProcessOperator(Scanner.token_t Operator){
			switch (Operator.token_string) {
			case Scanner.OperatorAdd:
				Console.WriteLine ("PEEKTOKEN: " + PeekToken (CurrentToken + 1).token_string);
				if (PeekToken (CurrentToken + 1).token_string == "(") {
					Scanner.token_t mytok = new Scanner.token_t(); 
					mytok = ReadToken ();
					while (mytok.token_string != ")") {
						mytok = ReadToken ();
						Console.WriteLine ("DEBUG: " + mytok.token_string);
						if (mytok.token_id != Scanner._OPERATOR && mytok.token_id != Scanner._LITERAL_NUM && mytok.token_id != Scanner._SEMICOLON && mytok.token_id != Scanner._BLOCKINDICATOR) {
							throw new Exception (string.Format ("Unknown Character!"));
						}
						ProcessToken (mytok);
					}
				} else {
					CodeGen.PushRegister (0);
					Console.WriteLine (PeekToken (CurrentToken).token_string);
					CodeGen.PushtoStack (Convert.ToInt32 (ReadToken ().token_string));
					CodeGen.DoAddOnStack ();
					CodeGen.PopRegister (0);
				}
				break;
			case Scanner.OperatorMul:
				CodeGen.PushRegister (0);
				CodeGen.PushtoStack (Convert.ToInt32 (ReadToken ().token_string));
				CodeGen.DoMulOnStack ();
				CodeGen.PopRegister (0);
				break;
			case Scanner.OperatorSub:
				CodeGen.PushRegister (0);
				CodeGen.PushtoStack (Convert.ToInt32 (ReadToken ().token_string));
				CodeGen.DoMulOnStack ();
				CodeGen.PopRegister (0);
				break;
			case Scanner.OperatorDivide:
				CodeGen.PushRegister (0);
				CodeGen.PushtoStack (Convert.ToInt32 (ReadToken ().token_string));
				CodeGen.DoDivOnStack ();
				CodeGen.PopRegister (0);
				break;
			default:
				Console.WriteLine (PeekToken (CurrentToken-1).token_string);
				CodeGen.LoadRegisterWithImmediate (0, Convert.ToInt32 (PeekToken(CurrentToken-1).token_string));
				break;
			}
		}
		public static Scanner.token_t ReadToken(){
			if (CurrentToken >= GlobalTokenList.Count) {
				Console.WriteLine ("Unexpected end of file");
				throw new Exception (string.Format ("Parser Error: Unexpected end of file reached"));
			} else {
				CurrentToken++;
				return GlobalTokenList [CurrentToken-1];
			}
		}
		public static Scanner.token_t PeekToken(int index){
			if (index > GlobalTokenList.Count) {
				return GlobalTokenList [GlobalTokenList.Count];
			} else {
				return GlobalTokenList [index];
			}
		}
	}
}

