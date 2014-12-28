#include <r3x_bios.h>
#include <nt_malloc.h>
#include <assert.h>
void r3x_load_bios(r3x_header_t* header, r3x_cpu_t* CPU) {
	// Load the bios, not really sure whatthefuck should i reallyfucking write here.
	FILE* biosfile = fopen("./bios/bios.bin", "r");
	if(biosfile == NULL) { printf("Error : bios/bios.bin is missing\n"); nt_freeall(); exit(1); } 
	else { 
		// read 512 bytes
		fseek(biosfile, 0L, SEEK_END);
		unsigned int totalsize = ftell(biosfile);
		fseek(biosfile, 0L, SEEK_SET);
		assert(CPU->Memory);
		unsigned int sizeread = fread(&CPU->Memory[0], sizeof(uint8_t), totalsize, biosfile);
		if(sizeread != REX_BIOS_SIZE) { 
			printf("What the fuck is this thing?\nIt's supposed to a BIOS for fucks sake.\nOr something wrong with fread?\nExpected size : %u, but read %u\n", REX_BIOS_SIZE, sizeread);	
		    exit(1);

		}
		header->r3x_init = 0;
		header->r3x_text_size = 512;
		fclose(biosfile);
	} 
}
