using System;

namespace cc
{
	public class ParserDeclaration
	{
		public static string CheckForFunctionDeclaration(){
			int FunctionOffset = Parser.CurrentToken;
			if (Parser.ReadToken().token_id == Scanner._KEYWORD_TYPE) {
				if (Parser.ReadToken ().token_id == Scanner._IDENTIFIER) {
					if (Parser.OpenParenThesis () == true) {
						return Parser.ReturnParserString (Parser.CurrentToken - 1);
					}
				}
			}
			// Maybe it's something like 'const int', 'unsigned int' etc..?
			Parser.CurrentToken = FunctionOffset;
			if (Parser.ReadToken ().token_id == Scanner._KEYWORD_TYPEDESC) {
				if (Parser.ReadToken().token_id == Scanner._KEYWORD_TYPE) {
					if (Parser.ReadToken ().token_id == Scanner._IDENTIFIER) {
						if (Parser.OpenParenThesis () == true) {
							return Parser.ReturnParserString (Parser.CurrentToken - 1);
						}
					}
				}
			}
			// Roll back
			Parser.CurrentToken = FunctionOffset;
			return null;
		}
		public static string CheckForTypeDeclaration(){
			int CurrentOffset = Parser.CurrentToken;
			if (Parser.ReadToken ().token_id == Scanner._KEYWORD_TYPE) {
				return null;
			}
			return null;
		}
	}
}

