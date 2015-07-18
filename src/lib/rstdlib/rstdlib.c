#define SO_NAME "rstdlib.so"
#define API_VER 1
#include <virtual-machine.h>
#include <nt_malloc.h>

r3x_cpu_t* CPU;

typedef struct {
	FILE** FilePointers;
	unsigned int TotalPointers;
} r_FileList;

r_FileList* FileList;

void Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
	nt_malloc_init(false);
	FileList = nt_malloc(sizeof(r_FileList));
	FileList->FilePointers = nt_malloc(sizeof(FILE*)*16);
	FileList->TotalPointers = 16;
}
uint32_t r_strlen(void) {
	return strlen((char*)GetLinearAddress(CPU, GetArgument(CPU, 1,1)));
}
uint32_t r_strcmp(void) {
	char* arg1 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* arg2 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 2,2));
	return strcmp(arg1, arg2);
}
uint32_t r_strcpy(void) {
	char* arg1 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* arg2 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 2,2));
	memset(arg2, 0, strlen(arg2));
	strcpy(arg1, arg2);
	return 0;
}
uint32_t r_strcat(void) {
	char* arg1 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* arg2 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 2,2));
	strcat(arg2, arg1);
	return 0;
}
uint32_t r_memset(void) {
	void* arg1 = (void*)GetLinearAddress(CPU, GetArgument(CPU, 1,3));
	int arg2 = (int)GetArgument(CPU, 2,3);
	size_t arg3 = (size_t)GetArgument(CPU, 3,3);
	memset(arg1, arg2, arg3);
	return 0;
}
uint32_t r_memcpy(void) {
	void* arg1 = (void*)GetLinearAddress(CPU, GetArgument(CPU, 1,3));
	void* arg2 = (void*)GetLinearAddress(CPU, GetArgument(CPU, 2,3));
	size_t arg3 = (size_t)GetArgument(CPU, 3,3);
	memcpy(arg1, arg2, arg3);
	return 0;
}
uint32_t r_fopen(void) {
	char* arg1 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* arg2 = (char*)GetLinearAddress(CPU, GetArgument(CPU, 2,2));
	for(unsigned int i = 1; i < FileList->TotalPointers; i++) {
		if(FileList->FilePointers[i] == NULL) {
			FileList->FilePointers[i] = fopen(arg1, arg2);
			if(FileList->FilePointers[i] == NULL) {
				return 0;
			} else { 
				return i;
			}
		}
	}
	// okay nothing is null, allocate new ones then
	FileList->TotalPointers += 16;
	FileList->FilePointers = nt_realloc(FileList->FilePointers, FileList->TotalPointers);
	// todo: remove code repitition, and find out a new way to do this without goto
	for(unsigned int i = 1; i < FileList->TotalPointers; i++) {
		if(FileList->FilePointers[i] == NULL) {
			FileList->FilePointers[i] = fopen(arg1, arg2);
			if(FileList->FilePointers[i] == NULL) {
				return 0;
			} else { 
				return i;
			}
		}
	}
	printf("[%s]: Internal Error. Cannot open file: %s, with mode %s\n", SO_NAME, arg1, arg2);
	return (uint32_t)-1;
}
uint32_t r_fclose(void) {
	unsigned int arg1 = (unsigned int)GetArgument(CPU, 1,1);
	if(arg1 >= FileList->TotalPointers) {
		return 0;
	}
	if(FileList->FilePointers[arg1] == NULL) {
		return 0;
	}
	fclose(FileList->FilePointers[arg1]);
	FileList->FilePointers[arg1] = NULL;
	return 1;
}
uint32_t r_fread(void) {
	unsigned int arg1 = (unsigned int)GetArgument(CPU, 1,3);
	size_t arg2 = (unsigned int)GetArgument(CPU, 2,3);
	void* arg3 = (void*)GetLinearAddress(CPU, GetArgument(CPU, 3,3));
	if(arg1 >= FileList->TotalPointers) { return 0; }
	if(FileList->FilePointers[arg1] == NULL) { return 0; }
	memset(arg3, 0, arg2);
	return (uint32_t)fread(arg3, 1, arg2, FileList->FilePointers[arg1]);
}
uint32_t r_fwrite(void) {
	unsigned int arg1 = (unsigned int)GetArgument(CPU, 1,3);
	size_t arg2 = (unsigned int)GetArgument(CPU, 2,3);
	void* arg3 = (void*)GetLinearAddress(CPU, GetArgument(CPU, 3,3));
	if(arg1 >= FileList->TotalPointers) { return 0; }
	if(FileList->FilePointers[arg1] == NULL) { return 0; }
	return (uint32_t)fwrite(arg3, 1, arg2, FileList->FilePointers[arg1]);
}
uint32_t r_fflush(void) {
	unsigned int arg1 = (unsigned int)GetArgument(CPU, 1,1);
	if(arg1 >= FileList->TotalPointers) { return 0; }
	if(FileList->FilePointers[arg1] == NULL) { return 0; }
	return !fflush(FileList->FilePointers[arg1]);
}
uint32_t r_getenv(void) {
	char* arg1 = GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* arg2 = GetLinearAddress(CPU, GetArgument(CPU, 2,2));
	if(getenv(arg1) != NULL) {
		if(arg2 != NULL) {
			memset(arg2, 0, strlen(getenv(arg1)));
			strcpy(arg2, getenv(arg1));
		}
		return strlen(getenv(arg1));
	}
	return 0;
}
uint32_t r_setenv(void) {
	char* arg1 = GetLinearAddress(CPU, GetArgument(CPU, 1,3));
	char* arg2 = GetLinearAddress(CPU, GetArgument(CPU, 2,3));
	int arg3 = GetArgument(CPU, 3,3);
	return setenv(arg1, arg2, arg3);
}
uint32_t r_exitlib(void) {
	nt_freeall();
	return 0;
}
uint32_t r_getapiversion(void) {
	return API_VER;
}
