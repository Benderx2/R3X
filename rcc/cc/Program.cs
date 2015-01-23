using System;
using System.Collections;
using System.Collections.Generic;
namespace cc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string mystring = "unsigned int main(void)/*Comment*/{\t\nprintf(\"hello world, %d\",27); int a=5; if(a<=6){printf(\"a is not 6\");}a++;return(32-4)&&82;}";
			List<string> mylist = new List<string> ();
			Tokenizer.TokenizeString (mystring, mylist);
			for (int i = 0; i < mylist.Count; i++) {
				Console.WriteLine (mylist [i]);
			}
			List<Scanner.token_t> myscan = Scanner.ScanTokens (mylist);
			for (int i = 0; i < myscan.Count; i++) {
				myscan [i].Display ();
			}
		}
	}
}
