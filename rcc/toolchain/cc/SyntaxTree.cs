using System;
using System.Collections.Generic;
namespace cc
{
	public class SyntaxTree
	{
		private int _NodeType;
		private Scanner.token_t _Token;
		private SyntaxTree _Parent; // It's own index
		private List<SyntaxTree> _ChildNodes;
		private bool _IsDirectory;
		public int NodeType {
				get {
					return _NodeType;
				} 
				set {
					_NodeType = value;
				}
			}
		public Scanner.token_t Token {
				get {
					return _Token;
				}
				set {
					_Token = value;
				}
			}
		public List<SyntaxTree> ChildNodes {
				get {
					return _ChildNodes;
				}
				set {
					_ChildNodes = value;
				}
			}
		public SyntaxTree Parent {
			get {
				return _Parent;
			}
			set {
				_Parent = value;
			}
		}
		public bool IsDirectory {
			get {
				return _IsDirectory;
			}
			set {
				_IsDirectory = value;
			}
		}
	}
}

