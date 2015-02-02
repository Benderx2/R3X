0  PRINT "Testing Division"
GOTOL mylabel
10 PRINT "Testing division"
LABEL mylabel
30 LET X = 20
40 LET Y = 5
50 LET Z = X / Y
60 PRINT "Div: "; Z
70 ASM "push 0"
80 ASM "exit"
