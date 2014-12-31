using System;

namespace cc
{
	public class ParserAssignment
	{
		public const int Equal = 0x0;
		public const int XorEqual = 0x01;
		public const int OrEqual = 0x02;
		public const int AndEqual = 0x03;
		public const int NegEqual = 0x04;
		public const int AddEqual = 0x05;
		public const int SubEqual = 0x06;
		public const int MulEqual = 0x07;
		public const int DivEqual = 0x08;
		public const int InvalidAssignment = 0x09;
		public static bool IsAssignment(){
			int currentOffset = Parser.CurrentToken;
			if (Parser.ReadToken ().token_id == Scanner._ASSIGNMENT) {
				Parser.CurrentToken = currentOffset;
				return true;
			}
			Parser.CurrentToken = currentOffset;
			return false;
		}
		public static int GetTypeOfAssignment(){
			int currentOffset = Parser.CurrentToken;
			int ReturnValue = 0;
			switch(Parser.ReadToken().token_string){
			case Scanner.AssignmentAdd:
				ReturnValue = AddEqual;
				break;
			case Scanner.AssignmentAnd:
				ReturnValue = AndEqual;
				break;
			case Scanner.AssignmentEqual:
				ReturnValue = Equal;
				break;
			case Scanner.AssignmentMul:
				ReturnValue = MulEqual;
				break;
			case Scanner.AssignmentNeg:
				ReturnValue = NegEqual;
				break;
			case Scanner.AssignmentOr:
				ReturnValue = OrEqual;
				break;
			case Scanner.AssignmentSub:
				ReturnValue = SubEqual;
				break;
			case Scanner.AssignmentXor:
				ReturnValue = XorEqual;
				break;
			default:
				ReturnValue = InvalidAssignment;
				break;
			}
			Parser.CurrentToken = currentOffset;
			return ReturnValue;
		}
	}
}

