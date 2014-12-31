using System;

namespace cc
{
	public class ParserConditional
	{
		public const int ConditionalIf = 0x0;
		public const int ConditionalWhile = 0x01;
		public static bool IsConditional(){
			int currentOffset = Parser.CurrentToken;
			if (Parser.ReadToken ().token_id == Scanner._CONDITIONAL) {
				Parser.CurrentToken = currentOffset;
				return true;
			}
			Parser.CurrentToken = currentOffset;
			return false;
		}
		public static int GetTypeOfConditional(){
			int currentOffset = Parser.CurrentToken;
			int ReturnValue = 0;
			switch(Parser.ReadToken().token_string){
			case Scanner.KeywordIf:
				ReturnValue = ConditionalIf;
				break;
			case Scanner.KeywordWhile:
				ReturnValue = ConditionalWhile;
				break;
			}
			Parser.CurrentToken = currentOffset;
			return ReturnValue;
		}
	}
}

