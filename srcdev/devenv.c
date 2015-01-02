/** R3X Development Enviroment **/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "nt_malloc.h"
#define SRC_DEV_VER "0.1.2 (Buffety Cutlet)"
#define SHELL_MAX_INPUT_SIZE 100
#define FILES_ON_LINE_MAX 8
char* ApplicationPath = NULL;
char* CurrentDirectory = NULL;
char str[SHELL_MAX_INPUT_SIZE];
void GetApplicationPath(void);
void shell(void);
int main(int argc, const char* argv[]){
	GetApplicationPath();
	nt_malloc_init(false);
	printf("REX Development Environment -- Version: %s\n", SRC_DEV_VER);
	printf("You have launched the REX Development environment, type 'help' for help.\n");
	(void)argc; (void)argv;
	CurrentDirectory = nt_malloc(strlen(ApplicationPath)+1);
	strcpy(CurrentDirectory, ApplicationPath);
	shell();
}
void GetApplicationPath(void) { 
	#ifdef __linux__
	char buf[FILENAME_MAX];
	memset(buf, 0, sizeof(buf));
	/* Note we use sizeof(buf)-1 since we may need an extra char for NUL. */
	if (readlink("/proc/self/exe", buf, sizeof(buf)-1) < 0)
	{
		/* There was an error...  Perhaps the path does not exist
		* or the buffer is not big enough.  errno has the details. */
		perror("readlink");
		printf("FATAL: Unable to read /proc/self/exe!");
		exit(EXIT_FAILURE);
	}
	ApplicationPath = malloc(strlen(buf)+1);
	strcpy(ApplicationPath, buf);
	for(int i = strlen(ApplicationPath); i != 0; i--){
		// Backwards babe...
		/** Remove the executable name from the path **/
		if(ApplicationPath[i] == '/'){
			memset((char*)((intptr_t)ApplicationPath + i), 0, strlen(ApplicationPath)-i);
			return;
		}
	}
	#else 
	#error "GetApplicationPath unimplemented for target, please write your own fucking implementation"
	#endif
}
void shell(void){
	while(true) {
		memset(str, 0, SHELL_MAX_INPUT_SIZE);
		printf("[%s]$ ", CurrentDirectory);
		char* input = fgets(str, SHELL_MAX_INPUT_SIZE-1, stdin);
		if(strncmp(input, "help", 4)==0){
			printf("R3X Development Environment, Version %s\n", SRC_DEV_VER);
			printf("quit: Quits this shell\n");
			printf("dir: Shows the contents of current directory\n");
			printf("cur: Prints the absolute location of current directory\n");
		} else if(strncmp(input, "quit", 4)==0){
			exit(EXIT_SUCCESS);
		} else if(strncmp(input, "dir", 3)==0){
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir (CurrentDirectory)) != NULL) {
				int linecount = 0;
				/* print all the files and directories within directory */
				while ((ent = readdir (dir)) != NULL) {
					printf ("%s\t", ent->d_name);
					linecount++;
					if(linecount == FILES_ON_LINE_MAX){
						printf("\n");
						linecount = 0;
					}
				}
				closedir (dir);
				printf("\n");
} 			else {
				/* could not open directory */
				perror ("");
			    exit(EXIT_FAILURE);
			}
		} else if(strncmp(input, "cur",3)==0){
			printf("%s\n", CurrentDirectory);
		} else if(strncmp(input, "\n", 1)==0){
		} else {
			printf("Try again!\n");
		}
	}
}
