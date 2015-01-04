/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <r3x_bios.h>
#include <nt_malloc.h>
#include <assert.h>
extern char* ApplicationPath;
void r3x_load_bios(r3x_cpu_t* CPU) {
	// Load the bios, not really sure whatthefuck should i reallyfucking write here.
	char* BIOSPath = nt_concat(ApplicationPath, "/bios/bios.bin");
	FILE* biosfile = fopen(BIOSPath, "r");
	if(biosfile == NULL) { perror("Unable to read BIOS image <vm dir>/bios/bios.bin"); nt_freeall(); exit(1); } 
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
		fclose(biosfile);
	} 
}
