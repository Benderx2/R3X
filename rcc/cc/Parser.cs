using System; 
using System.Collections.Generic;
namespace cc
{
	public class Parser
	{
		public struct PSymbol {
			int SymType;
			string SymName;
		}
		public static int Parse(List <Scanner.token_t> Tokens){
			int _lbl_rand = 0;
			for (int i = 0; i < Tokens.Count; i++) {
				if (Tokens [i].token_string == Scanner.InternalFunctionPrint && Tokens [i].token_id == Scanner._INTERNAL_FUNC) {
					Console.WriteLine ("section .data { __data" + _lbl_rand.ToString () + ": db '" + Tokens [i + 1].token_string + "', 0 }\n");
					Console.WriteLine ("section .text {\npush __" + _lbl_rand.ToString () + "\nsyscall SYSCALL_PUTS\n}\n");
					_lbl_rand++;
				} else if (Tokens [i].token_string == Scanner.KeywordChar && Tokens[i].token_id == Scanner._KEYWORD_TYPE) {
					//! Check for assignment
					string TokenName = Tokens [i + 1].token_string;
					if (Tokens [i + 2].token_id != Scanner._ASSIGNMENT) {
						//! Declare in BSS
						Console.WriteLine ("section .bss { __char_" + TokenName.ToString () + ": db 0 }\n");
					} else {
						Console.WriteLine("section .data { __char_" + TokenName.ToString() + ": db " + Tokens[i+3].token_string + " }\n");  
					}
				}
			}
			return 0;
		}
	}
}

