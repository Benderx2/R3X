using System;
using System.Collections;
using System.Collections.Generic;
namespace cc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string mystring = "5+3+(2+7);";
			List<string> mylist = new List<string> ();
			Tokenizer.TokenizeString (mystring, mylist);
			List<Scanner.token_t> myscan = Scanner.ScanTokens (mylist);
			Parser.ParserInit (myscan);
			Parser.ParseExpression ();
		}
	}
}
