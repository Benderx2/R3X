using System;
using System.Collections;
using System.Collections.Generic;
namespace cc
{
	public class Tokenizer
	{
		public static int TokenizeString(string tokens, List<string> tokenlist) {
			int i = 0;
			char a = ' ';
			string tempbuf = "";
			while(i < tokens.Length)
			{
				tempbuf = "";
				// Skip newlines...
				while (a == 0 || a == '\n' || a == '\t' || a == '\r' || a == ' ') { /* just make sure we don't overflow! */ if (i >= tokens.Length) { return 0; } a = tokens[i]; i++; }
				i--;
				while (a != 0 && a != '\n' && a != '\t' && a != '\r' && a != ' ' && i < tokens.Length) {
					a = tokens [i];
					if (a == '/' && tokens[i+1] == '*') {
						a = 'x';
						i += 2;
						while (a != '*' && tokens[i+1] != '/') {
							if (i >= tokens.Length) {
								return 0;
							}
							a = tokens [i];
							i++;
						}
						i += 2;
					}
					// we need to handle strings differently...
					else if (a == '"') {
						// Add whatever is in dynamic str
						if (String.IsNullOrWhiteSpace (tempbuf) == false) {
							tokenlist.Add (tempbuf);
						}
						a = ' ';
						tempbuf = "\"";
						i++;
						while (a!='"') {
							a = tokens [i];
							tempbuf = tempbuf + a;
							i++;
						}
						if (String.IsNullOrWhiteSpace (tempbuf) == false) {
							tokenlist.Add (tempbuf);
						}
						tempbuf = "";
					} else if (a == ';') {
						if (String.IsNullOrWhiteSpace (tempbuf) == false) {
							tokenlist.Add (tempbuf);
						}
						tokenlist.Add (";");
						tempbuf = "";
						i++;
					} else if (a == '+' || a == '-' || a == '*' || a == '/' || a == '&' || a == '^' || a == '!' || a == '>' || a == '<' || a == '=' || a == ',' || a == '%') {
						if (String.IsNullOrWhiteSpace (tempbuf) == false) {
							tokenlist.Add (tempbuf);
						}
						// Check if the second operator is same as the first, if then it MAY be a logical operator.
						if (tokens [i + 1] == a) {
							tempbuf = a.ToString () + tokens [i + 1].ToString ();
							tokenlist.Add (tempbuf);
							i++;
							// Handle it different for '!='
						} else if (a == '!' && tokens [i + 1] == '=') {
							tempbuf = "!=";
							tokenlist.Add (tempbuf);
							i++;
						} else if (a == '<' || a == '>') {
							if (tokens [i + 1] == '=') {
								tokenlist.Add (a.ToString () + tokens [i + 1].ToString ());
								i++;
							}
						}
						// It's an operator add it.
						else {
							tokenlist.Add (a.ToString ());
						}
						// Set string to empty
						tempbuf = "";
						i++;
					}
					// Is it a bracket?
					else if (a == '(' || a == ')' || a == '[' || a == ']' || a == '{' || a == '}') {
						// Add whatever was take and 
						if (String.IsNullOrWhiteSpace (tempbuf) == false) {
							tokenlist.Add (tempbuf);
						}
						// Add the bracket
						tokenlist.Add (a.ToString());
						// Set the string to null
						tempbuf = "";
						i++;
					// Ignore white spaces
					} else if (a == ' ' || a == '\n' || a == '\r' || a == '\t') {
					}
					// Else keep adding character to string
					else {
						tempbuf = tempbuf + a;
						i++;
					}
				}
				// Just a sanity check, if string is not empty then only add the token.
				if (String.IsNullOrWhiteSpace(tempbuf)==false) {
					tokenlist.Add (tempbuf);
					tempbuf = "";
				}
			}
			return 0;
		}
	}
}

