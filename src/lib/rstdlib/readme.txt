this file contains the rstdlib implementation for r3x and examples in t++.
STRING FUNCTIONS:

@r_strlen(string) -> return length of string

@r_strcmp(str1, str2) -> compare 2 strings, returns 0 (true) if they are equal.

@r_strcpy(dest, src) -> Copy string src into string dest.

@r_strcat(dest, src) -> concatenate two strings, (note dest should have required buffer size)

MEMORY ACCESS FUNCTIONS

@r_memset(buffer, char, size) -> set all bytes in buffer of size to char.

@r_memcpy(dest, src, length) -> copy src to dest of length "length".

FILE I/O FUNCTIONS

@r_fopen(filename, mode) -> open file with name "filename" and mode. returns file descriptor (filefd)

@r_fclose(filefd) -> close a file

@r_fread(filefd, length, buffer) -> read from filfd into buffer of length "length". return the amount of bytes
read

@r_fwrite(filefd, length, buffer) -> write to filefd of length "length" from buffer. return the amount of bytes
written

@r_fflush(filefd) -> flush filefd.

ENVIRONMENT FUNCTIONS

@r_getenv(envname, buf) -> copy value of environment variable "envname" to buf. buffer should be allocated previously.
in order to find the required size of buffer, use the function with null as buf, it will return the amount of bytes
required for envname.

@r_setenv(envname, envval, overwrite) -> set environment variable "envname" to "envval", with "overwrite" properties

MATH FUNCTIONS

@r_sin/r_cos/r_tan/r_sec/r_cosec/r_cot(float) -> return sine/cos/tan/sec/cosec/cot in [radians]
@r_sinh/r_cosh/r_tanh/r_sech/r_cosech/r_coth(float) -> return value of hyperbolic trig functions [in radians]
@r_asin/r_acos/r_atan/r_asec/r_acosec/r_acot(float) -> return value of inverse trig functions in radians
@r_asinh/r_acosh/r_atanh/r_asech/r_acoseh/r_coth(float) -> do i need to explain this

@r_exp(float) -> return e^float

@r_log(float) -> return log base e of float
@r_log10(float) -> return log base 10 of float

@r_pow(base, exp) -> return base^exp
@r_ceil(float) -> return ceil of float
@r_floor(float) -> return floor of float
@r_fmod(dividend, divisor) -> return remainder of dividend/divisor

SOCKET FUNCTIONS:

@r_connect(server_name, port) -> Connects to the server with the specified port and returns the socket descriptor.

@r_send(sockfd, string) -> sends a null terminated "string" to sockfd with a newline '\n'

@r_receive(sockfd, length, buffer) -> Reads from socket sockfd, to buffer with specified length. This function will
!WAIT! until data is received. returns the number of bytes read.

@r_receive_no_wait(sockfd, length, buffer) -> Reads from socket sockfd, to buffer with specified length. This function does not wait for data to be received. if there is no data received, it returns 0, else it returns the
number of bytes read.
