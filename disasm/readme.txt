This folder contains a couple of tools for the R3X Virtual Machine.
There are three files in this directory:
readrex.c - Useful for reading and parsing headers of shared libraries/executables
rexdump.c - Useful for disassembling executables (shared libraries not currently supported)
cgen.c - Generates a checksum of the executable, the checksum generated uses crc32 and hence is not cryptographically
secure.
Pass the '-h' option to the program for help.
