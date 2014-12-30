using System;
using System.IO;
using System.Collections.Generic;
namespace cc
{
	public class CodeGen
	{
		public struct machine_t {
			public string name;
			public int number_of_registers;
			public List<string> registers;
			public int WordSize;
			public bool UseABIUnderscores;
		}
		public static string CodeHeader = "";
		public static string CodeFooter = "";
		public static machine_t Target;
		public static int CurrentLabel = 0;
		public static int DataOffset = 0;
		public static int BSSOffset = 0;
		private static StreamWriter stream;
		public static StreamWriter OpenWriteStream(string name){
			File.Create (name);
			StreamWriter file = new System.IO.StreamWriter (name);
			file.AutoFlush = true;
			return file;
		}
		public static void WriteStream(string towrite){
			stream.WriteLine (towrite);
		}
		public static void CodeGenInit(int argc, string[] argv){
			stream = OpenWriteStream (argv[1]);
			Target = new machine_t ();
			Target.name = "R3X System";
			Target.WordSize = 4;
			Target.number_of_registers = 21;
			Target.registers.Add ("R0");
			Target.registers.Add ("R1");
			Target.registers.Add ("R2");
			Target.registers.Add ("R3");
			Target.registers.Add ("R4");
			Target.registers.Add ("R5");
			Target.registers.Add ("R6");
			Target.registers.Add ("R7");
			Target.registers.Add ("R8");
			Target.registers.Add ("R9");
			Target.registers.Add ("R10");
			Target.registers.Add ("R11");
			Target.registers.Add ("R12");
			Target.registers.Add ("R13");
			Target.registers.Add ("R14");
			Target.registers.Add ("R15");
			Target.registers.Add ("R16");
			Target.registers.Add ("R17");
			Target.registers.Add ("R18");
			Target.registers.Add ("R19");
			Target.registers.Add ("R20");
			CodeHeader = "include \"libR3X/libR3X.inc\"";
			CodeFooter = "end";
			Target.UseABIUnderscores = true;
			stream.Write (CodeHeader);
		}
		public static void GenFinalize(){
			stream.Write (CodeFooter);
		}
		public static string GenerateFunctionLabel(string functionname) {
			if (Target.UseABIUnderscores == true) {
				stream.WriteLine ("_" + functionname + ":");
				return "_" + functionname;
			} else {
				stream.WriteLine (functionname + ":");
				return functionname;
			}
		}
		public static int GenerateLabel() {
			stream.WriteLine ("__label" + CurrentLabel.ToString () + ":");
			CurrentLabel++;
			return CurrentLabel-1;
		}
		/** Now begins the system specific code generator **/
		public static void TextSection(){
			stream.WriteLine (".text {");
		}
		public static void DataSection(){
			stream.WriteLine (".data {");
		}
		public static void BSSSection(){
			stream.WriteLine (".bss {");
		}
		public static void EndSection(){
			stream.WriteLine ("}");
		}
		public static void PushtoStack(int value){
			stream.WriteLine ("push " + value.ToString ());
		}
		public static void PopFromStack(){
			stream.WriteLine ("pop");
		}
		public static void PushFloattoStack(float value) {
			stream.Write ("push " + value.ToString () + "f");
		}
		public static void PushRegister(int register_index){
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("pushr " + Target.registers [register_index]);
		}
		public static void PopRegister(int register_index){
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("popr " + Target.registers [register_index]);
		}
		public static void CompareRegisterAndSetFlag(int register_index, int val) {
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("cmpr " + Target.registers [register_index] + ", " + val.ToString ());
		}
		public static void CompareRegistertoRegisterAndSetFlag(int register_index, int register_index2) {
			if (register_index > Target.number_of_registers || register_index2 > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("cmpr " + Target.registers [register_index] + ", " + Target.registers[register_index2]);
		}
		public static void JumpIfEqual(int val, int label){
			stream.WriteLine ("je __label" + label.ToString ());
		}
		public static void JumpIfZero(int label) {
			stream.WriteLine ("jz __label" + label.ToString ());
		}
		public static void JumpIfGreater(int label) {
			stream.WriteLine ("jg __label" + label.ToString ());
		}
		public static void JumpIfLesser(int label) {
			stream.WriteLine ("jl __label" + label.ToString ());
		}
		public static void JumpIfLesserOrEqual(int label) {
			stream.WriteLine ("jle __label" + label.ToString ());
		}
		public static void JumpIfGreaterOrEqual(int label) {
			stream.WriteLine ("jge __label" + label.ToString ());
		}
		public static void MachineCallLabel(int label) {
			stream.WriteLine ("call __label" + label.ToString ());
		}
		public static void MachineCallFunction(string functionname) {
			if (Target.UseABIUnderscores == true) {
				stream.WriteLine ("call _" + functionname);
			} else {
				stream.WriteLine ("call " + functionname);
			}
		}
		public static void MachineReturn(){
			stream.WriteLine ("ret");
		}
		public static void LoadRegisterWithImmediate(int register_index, int value){
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("loadr " + Target.registers [register_index] + ", " + value.ToString());
		}
		public static void LoadRegisterWithImmediateFloat(int register_index, int value){
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("loadr " + Target.registers [register_index] + ", " + value.ToString() + "f");
		}
		public static void LoadRegisterWithRegister(int register_index, int register_index2){
			if (register_index > Target.number_of_registers || register_index2 > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("loadrr " + Target.registers [register_index] + ", " + Target.registers[register_index2]);
		}
		public static void LoadRegisterWithMemoryAddress(int size, int register_index, int addr){
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			string sizes = "";
			if (size == 1) {
				sizes = "byte";
			} else if (size == 2) {
				sizes = "word";
			} else {
				sizes = "dword";
			}
			stream.WriteLine ("loadrm " + sizes + ", " + Target.registers [register_index] + ", " + addr.ToString());
		}
		public static void PushtoStackfromStackOffset(int stack_off){
			stream.WriteLine ("loads " + stack_off.ToString ());
		}
		public static void PushtoCallStack(int val) {
			stream.WriteLine ("pusha " + val.ToString());
		}
		public static void PopFromCallStack() {
			stream.WriteLine ("popa");
		}
		public static void PushRegistertoCallStack(int register_index) {
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("pushar " + Target.registers [register_index]);
		}
		public static void PopRegisterFromCallStack(int register_index) {
			if (register_index > Target.number_of_registers) {
				Console.WriteLine ("Fatal Error: Invalid register Index for target machine");
				return;
			}
			stream.WriteLine ("popar " + Target.registers [register_index]);
		}
		public static void DoBitwiseAndOnStack(){
			stream.WriteLine ("and");
		}
		public static void DoBitwiseOrOnStack(){
			stream.WriteLine ("or");
		}
		public static void DoBitwiseXorOnStack(){
			stream.WriteLine ("xor");
		}
		public static void DoBitwiseNegOnStack(){
			stream.WriteLine ("neg");
		}
		public static void DoBitwiseNotOnStack(){
			stream.WriteLine ("not");
		}
		public static void DoAddOnStack() {
			stream.WriteLine ("add");
		}
		public static void DoSubOnStack() {
			stream.WriteLine ("sub");
		}
		public static void DoMulOnStack() {
			stream.WriteLine ("mul");
		}
		public static void DoDivOnStack() {
			stream.WriteLine ("div");
		}
		public static void DoFAddOnStack() {
			stream.WriteLine ("fadd");
		}
		public static void DoFSubOnStack() {
			stream.WriteLine ("fsub");
		}
		public static void DoFMulOnStack() {
			stream.WriteLine ("fmul");
		}
		public static void DoFDivOnStack() {
			stream.WriteLine ("fdiv");
		}
		public static int DeclareEmptyData(int size) {
			int currentDataOff = BSSOffset;
			BSSSection ();
			if (size == 1) {
				BSSOffset += 1;
				stream.WriteLine ("rb 0");
			} else if (size == 2) {
				BSSOffset += 2;
				stream.WriteLine ("rw 0");
			} else {
				BSSOffset += 4;
				stream.WriteLine ("rd 0");
			}
			EndSection ();
			return DataOffset;
		}
		public static void DeclareData(int size, int value) {
			int currentDataOff = DataOffset;
			DataSection ();
			if (size == 1) {
				DataOffset += 1;
				stream.WriteLine ("db " + value.ToString());
			} else if (size == 2) {
				DataOffset += 2;
				stream.WriteLine ("dw " + value.ToString());
			} else {
				DataOffset += 3;
				stream.WriteLine ("dd " + value.ToString());
			}
			EndSection ();
			return currentDataOff;
		}
		public static int DeclareString(int size, string value) {
			int currentDataoff = DataOffset;
			DataSection ();
			stream.WriteLine ("db '" + value + "', 0");
			DataOffset += value.Length + 1; // add 1 for 0 character
			EndSection ();
			return currentDataoff;
		}

	}
}

