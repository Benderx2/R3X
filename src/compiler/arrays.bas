PRINT "Array stuff"
REM Allocate 256 bytes for S.
ALLOC S = 256
REM Unfortunately, only 8-bit arrays are supported.
LET [S] = 'A'
LET [S+1] = 0
LET X = [S]
PRINT "S[0] = "; X
END
