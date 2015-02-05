PRINT "Array stuff"
REM Allocate 4 bytes for S.
ALLOC S = 4
REM {} = 32-bit array access, @@ = 16-bit array access, [] = 8-bit array access
LET {S} = 41089340
LET X = [S]
PRINT "S[0] = "; X, {S}
END
