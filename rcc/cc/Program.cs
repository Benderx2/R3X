using System; 
using System.Collections;
using System.Collections.Generic;
namespace cc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string mystring = "char a; char b = 'B'; @print \"hello, world!\"";
			Console.WriteLine ("Input is: \n" + mystring);
			Console.WriteLine ("Tokenizer Output:\n");
			List<string> mylist = new List<string> ();
			Tokenizer.TokenizeString (mystring, mylist);
			for (int i = 0; i < mylist.Count; i++) {
				Console.WriteLine (mylist [i]);
			}
			List<Scanner.token_t> myscan = Scanner.ScanTokens (mylist);
			for (int i = 0; i < myscan.Count; i++) {
				myscan [i].Display ();
			}
			Parser.Parse (myscan);
		}
	}
}
