/** R3X Stream manager **/
#include <r3x_stream.h>
FILE** streams = NULL;
unsigned int number_of_total_streams = 16;
unsigned int used_streams = 0;
void init_stream_manager(void) { 
	streams = nt_malloc(sizeof(FILE*)*number_of_total_streams);
}
unsigned int stream_open(char* name) { 
	if(used_streams >= number_of_total_streams) { 
		streams = nt_realloc(streams, sizeof(FILE*)*(number_of_total_streams+16));
		number_of_total_streams += 16;
	}
	for(unsigned int i = 0; i < number_of_total_streams; i++)
	{
		if(streams[i] == NULL) { 
			streams[i] = fopen(name, "r+b");
			if(streams[i] == NULL) {
				printf("Unable to open stream... %s\n", name);
				return 0xFFFFFFFF;
			}
			else {
				printf("Opened stream for writing, handle: %u, name: %s\n", i, name);
				used_streams++;
				return i;
			}
		}
	}
	printf("Unable to find free stream! Please report this bug!\n");
	exit(EXIT_FAILURE);
}
void stream_close(unsigned int handle) { 
	if(handle >= number_of_total_streams) { 
		return;	
	} else {
		if(streams[handle] != NULL) {
			fclose(streams[handle]);
			streams[handle] = NULL;
			used_streams--;
		}	
	}
}
void stream_seek(unsigned int handle, long int off, int origin) { 
	if(handle >= number_of_total_streams) { 
		return;	
	} else {
		if(streams[handle] != NULL) {
			switch(origin) {
				case 0:
					fseek(streams[handle], off, SEEK_SET);
				case 1:
					fseek(streams[handle], off, SEEK_CUR);
				case 2:
					fseek(streams[handle], off, SEEK_END);
				default:
					return;			
			}
		}	
	}
}
unsigned int stream_tell(unsigned int handle) { 
	if(handle >= number_of_total_streams) { 
		return 0xFFFFFFFF;	
	} else {
		if(streams[handle] != NULL) {
			return (unsigned int)ftell(streams[handle]);
		}
	}
	return 0xFFFFFFFF;
}
unsigned int stream_read(void* ptr, unsigned int handle, size_t count) { 
	if(handle >= number_of_total_streams) {
		return 0xFFFFFFFF;
	} else {
		if(streams[handle] != NULL){
			return fread(ptr, sizeof(uint8_t), count, streams[handle]);
		} 
	}
	return 0xFFFFFFFF;
}
unsigned int stream_write(void* ptr, unsigned int handle, size_t count) { 
	if(handle >= number_of_total_streams) {
		return 0xFFFFFFFF;
	} else {
		if(streams[handle] != NULL){
			return fwrite((const void*)ptr, sizeof(uint8_t), count, streams[handle]);
		} 
	}
	return 0xFFFFFFFF;
}

