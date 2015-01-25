include 'libR3X/libR3X.pkg'
.text {
	function main
		Console.Write "I'm an asshole program!"
		Console.NewLine
		Console.Write "I'm going to write to executable memory!"
		Console.NewLine
		loadr R0, _lel
		stosd ; BOOBS. er. I mean BOOM
	_lel:
		dd 0
	endfunction main
}
end
