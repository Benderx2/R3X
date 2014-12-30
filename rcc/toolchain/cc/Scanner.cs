using System;
using System.Collections;
using System.Collections.Generic;
namespace cc
{
	public class Scanner
	{
		public const int _KEYWORD = 0;
		public const int _KEYWORD_TYPEDESC = 1;
		public const int _KEYWORD_TYPE = 2;
		public const int _OPERATOR = 3;
		public const int _BLOCKINDICATOR = 4;
		public const int _CONSTANT_STRING = 5;
		public const int _IDENTIFIER = 6;
		public const int _LITERAL_NUM = 7;
		public const int _LITERAL_STRING = 8;
		public const int _TOKEN_ID_MAX = 9;
		public const string StartBlock = "{";
		public const string EndBlock = "}";
		public const string StartFunctionBlock = "(";
		public const string EndFunctionBlock = ")";
		public const string StartArrayBlock = "[";
		public const string EndArrayBlock = "]";
		public const string OperatorDereferenceMulOrTypeCast = "*";
		public const string EndStatement = ";";
		public const string OperatorAdd = "+";
		public const string OperatorSub = "-";
		public const string OperatorMul = "*";
		public const string OperatorDivide = "/";
		public const string OperatorModulo = "%";
		public const string OperatorBitwiseAndOrAddressOf = "&";
		public const string OperatorBitwiseOr = "|";
		public const string OperatorBitwiseXor = "^";
		public const string OperatorBitwiseNot = "!";
		public const string OperatorBitwiseNeg = "~";
		public const string OperatorLogicalAnd = "&&";
		public const string OperatorLogicalOr = "||";
		public const string OperatorLeftShift = "<<";
		public const string OperatorRightShift = ">>";
		public const string OperatorGreaterThan = ">";
		public const string OperatorLesserThan = "<";
		public const string OperatorGreaterOrEqualto = ">=";
		public const string OperatorLesserOrEqualto = "<=";
		public const string OperatorLogicalEquality = "==";
		public const string OperatorLogicalInequality = "!=";
		public const string OperatorComma = ",";
		public const string KeywordConst = "const";
		public const string KeywordVolatile = "volatile";
		public const string KeywordSigned = "signed";
		public const string KeywordUnsigned = "unsigned";
		public const string KeywordVoid = "void";
		public const string KeywordInt = "int";
		public const string KeywordChar = "char";
		public const string KeywordFloat = "float";
		public const string KeywordBool = "bool";
		public const string KeywordTrue = "true";
		public const string KeywordFalse = "false";
		public const string KeywordReturn = "return";
		public const string KeywordWhile = "while";
		public const string KeywordIf = "if";
		public const string KeywordElse = "else";
		public const string KeywordAsm = "__asm__";
		public const string KeywordSizeof = "sizeof";
		public struct token_t
		{
			private int _token_id;
			private string _token_string;
			public int token_id {
				get {
					return _token_id;
				}
				set {
					if (value < _TOKEN_ID_MAX) {
						_token_id = value;
					}
				}
			}
			public string token_string {
				get {
					return _token_string;
				}
				set {
					_token_string = value;
				}
			}
			public void Display(){
				Console.WriteLine ("Token is: " + _token_string + " ID: " + ReturnTokenDesc(_token_id));
			}
			public string ReturnTokenDesc(int token) {
				if (token == _IDENTIFIER) {
					return "Identifier";
				} else if (token == _LITERAL_NUM) {
					return "Literal (Integer)";
				} else if (token == _LITERAL_STRING) {
					return "Literal (String)";
				} else if (token == _CONSTANT_STRING) {
					return "Constant (String)";
				} else if (token == _BLOCKINDICATOR) {
					return "Block Indicator";
				} else if (token == _KEYWORD_TYPE) {
					return "Type Keyword";
				} else if (token == _KEYWORD) {
					return "Reserved Keyword";
				} else if (token == _OPERATOR) {
					return "Operator";
				} else if (token == _KEYWORD_TYPEDESC) {
					return "Type description";
				} else {
					return "Unknown Token";
				}
			}
		}
		public static List<token_t> ScanTokens(List<string> Tokens){
			List<token_t> ScannerList = new List<token_t> ();
			token_t temptok = new token_t();
			int i = 0;
			while(i < Tokens.Count) {
				switch (Tokens [i]) {
				case KeywordVoid:
				case KeywordChar:
				case KeywordInt:
				case KeywordFloat:
				case KeywordBool:
					temptok.token_id = _KEYWORD_TYPE;
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				case KeywordConst:
				case KeywordVolatile:
				case KeywordSigned:
				case KeywordUnsigned:
					temptok.token_id = _KEYWORD_TYPEDESC;
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				case StartBlock:
				case StartFunctionBlock:
				case StartArrayBlock:
				case EndBlock:
				case EndFunctionBlock:
				case EndArrayBlock:
					temptok.token_id = _BLOCKINDICATOR;
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				case OperatorAdd:
				case OperatorBitwiseAndOrAddressOf:
				case OperatorBitwiseNeg:
				case OperatorBitwiseNot:
				case OperatorBitwiseOr:
				case OperatorBitwiseXor:
				case OperatorComma:
				case OperatorDereferenceMulOrTypeCast:
				case OperatorDivide:
				case OperatorModulo:
				case OperatorSub:
				case OperatorLogicalAnd:
				case OperatorLogicalEquality:
				case OperatorLogicalInequality:
				case OperatorLogicalOr:
				case OperatorLeftShift:
				case OperatorRightShift:
				case OperatorLesserThan:
				case OperatorGreaterThan:
				case OperatorLesserOrEqualto:
				case OperatorGreaterOrEqualto:
				case EndStatement:
					temptok.token_id = _OPERATOR;
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				case KeywordIf:
				case KeywordWhile:
				case KeywordElse:
				case KeywordReturn:
				case KeywordAsm:
				case KeywordSizeof:
				case KeywordTrue:
				case KeywordFalse:
					temptok.token_id = _KEYWORD;
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				default:
					if (Tokens [i] [0] == '"') {
						temptok.token_id = _LITERAL_STRING;
					} else if (IsNumeric (Tokens [i]) == true) {
						temptok.token_id = _LITERAL_NUM;
					} else {
						temptok.token_id = _IDENTIFIER;
					}
					temptok.token_string = Tokens [i];
					ScannerList.Add (temptok);
					i++;
					break;
				}
			}
			return ScannerList;
		}
		private static bool IsNumeric(string str){
			int n = 0;
			return int.TryParse(str, out n);
		}
	}
}

