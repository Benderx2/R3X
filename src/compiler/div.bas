0  PRINT "Testing Division and bitwise"
GOTOL mylabel
10 PRINT "Testing division"
LABEL mylabel
30 LET X = 20
40 LET Y = 5
50 LET Z = X / Y
60 PRINT "Div: "; Z
70 LET A = 50 & 2
80 PRINT "Or: "; A
90 ASM "push 0"
100 ASM "exit"
