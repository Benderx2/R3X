#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../src/include/r3x_format.h"
#include "../src/include/r3x_checksum_generator.h"
char* ExecutableName = NULL;

void ParseArguments(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    ParseArguments(argc, argv);
    if(ExecutableName==NULL) {
      printf("Pass -h option for help\n");
      exit(0);
    }
    FILE* InputFile = fopen(ExecutableName, "r+b");
    if(InputFile == NULL) {
      fprintf(stderr, "cgen: cannot open: %s\n", ExecutableName);
      exit(0);
    }
    fseek(InputFile, 0L, SEEK_END);
    unsigned int FileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    uint8_t* FileBuf = malloc(FileSize+1);
    if(fread(FileBuf, sizeof(uint8_t), FileSize, InputFile)!=FileSize){
      fprintf(stderr, "cgen: unable to read %u bytes from file: %s\n", FileSize, ExecutableName);
      exit(0);
    }
    r3x_header_t* Header = (r3x_header_t*)&FileBuf[0];
    if(Header->checksum != 0) {
      fprintf(stderr, "Error: Checksum already exists\n");
      exit(0);
    }
    fseek(InputFile, offsetof(r3x_header_t, checksum), SEEK_SET);
    uint32_t CRC32Checksum = GenerateChecksumCRC32(FileBuf, FileSize);
    fwrite(&CRC32Checksum, sizeof(uint32_t), 1, InputFile);
    printf("Generated checksum: %u\n", CRC32Checksum);
    printf("Checksum written to file\n");
    return 0;
}
void ParseArguments(int argc, char* argv[]) {
    int i = 0;
    for(i = 0; i < argc; i++) {
      if(!strcmp(argv[i], "-exe")) {
	if(i+1 < argc) {
	  ExecutableName = argv[i+1];
	} else {
	  printf("-exe option: Executable not specified\n");
	}
      } else if(!strcmp(argv[i], "-h")) {
	printf("cgen - Checksum generator for R3X programs.\n");
	printf("Generates a CRC32 checksum for an R3X executable\n");
	printf("Usage: cgen -exe <file name>\n");
	exit(0);
      }
    }
}