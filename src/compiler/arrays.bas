PRINT "Array stuff"
REM Allocate 4 bytes for S.
ALLOC S = 4
REM {} = 32-bit array access, @@ = 16-bit array access, [] = 8-bit array access
LET B = 100
REM 122233 = Decimal #FFF2FFF = Hex, `10101111 = Binary
LET {S} = #A10F2CD+#100ABC*S+(`1010111001)*B
LET X = [S]
PRINT "(uint8_t)S[0] and (uint32_t)S[0] = "; X, {S}
END
