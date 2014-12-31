using System;
using System.Collections.Generic;
namespace cc
{
	public class SyntaxTreeBuilder
	{
		// 0x0 for root
		public const int NODE_ROOT = 0x0;
		// Function node
		public const int NODE_FUNCTION = 0x01;
		// Conditional Node
		public const int NODE_WHILE = 0x02;
		public const int NODE_IF = 0x03;
		public const int NODE_ELSEIF = 0x4;
		public const int NODE_ELSE = 0x5;
		public const int NODE_ASSIGNMENT = 0x6;
		public const int NODE_TYPECAST = 0x7;
		public const int NODE_DECLARATION = 0x8;
		public const int NODE_STATEMENT = 0x9;
		public const int NODE_CONDITION = 0xA;
		public static SyntaxTree CreateRootNode() {
			SyntaxTree rootnode = new SyntaxTree();
			rootnode.ChildNodes = new List<SyntaxTree> ();
			rootnode.NodeType = NODE_ROOT;
			rootnode.IsDirectory = true;
			return rootnode;
		}
		public static void CreateChildNode(int type, SyntaxTree Node){
			if (type == NODE_ROOT) {
				Console.WriteLine ("Error: Attempt to build a root node inside a directory");
			} else if(Node.IsDirectory == false){
				Console.WriteLine ("Node not a directory");
			} else {
				SyntaxTree mytree = new SyntaxTree ();
				mytree.NodeType = type;
				mytree.Parent = Node;
				mytree.IsDirectory = false;
				Node.ChildNodes.Add (mytree);
			}
		}
		public static void CreateChildDirectory(int type, SyntaxTree Node){
			if (type == NODE_ROOT) {
				Console.WriteLine ("Error: Attempt to build a root node inside a directory");
			} else if(Node.IsDirectory == false){
				Console.WriteLine ("Node not a directory");
			} else {
				SyntaxTree mytree = new SyntaxTree ();
				mytree.NodeType = type;
				mytree.Parent = Node;
				mytree.IsDirectory = true;
				Node.ChildNodes.Add (mytree);
			}
		}
	}
}

