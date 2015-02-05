PRINT "Testing Division and bitwise"
GOTOL mylabel
PRINT "Testing division"
LABEL mylabel
LET X = 21
LET Y = 5
LET Z = X / Y
LET R = X%Y
PRINT "Mod: "; R
PRINT "Div: "; Z
LET A = 50 & 2
PRINT "Or: "; A
INPUT Y
PRINT "Y = "; Y
INPUT Y
ASM "push 0"
ASM "exit"
