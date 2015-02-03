PRINT "Testing Division and bitwise"
GOTOL mylabel
PRINT "Testing division"
LABEL mylabel
LET X = 20
LET Y = 5
LET Z = X / Y
PRINT "Div: "; Z
LET A = 50 & 2
PRINT "Or: "; A
ASM "push 0"
ASM "exit"
