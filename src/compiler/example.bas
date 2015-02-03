REM Fibonacci generator
LET I = 1
LET X = 1
LET Y = 0
WHILE I <= 10
	PRINT "FIB: "; I, X

	LET Z = X
	LET X = X + Y
	LET Y = Z
	
	LET I = I + 1

ENDW
ASM ".data \{ mystr : db 'Thank You!', 0 \}"
ASM "push mystr"
ASM "syscall SYSCALL_PUTS"
END
