/** R3X Mini-Server Enviroment **/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/socket.h>
extern char** environ;
#define SRC_DEV_VER "0.1.8a"
#define SHELL_MAX_INPUT_SIZE 100
#define FILES_ON_LINE_MAX 8
#define DEFAULT_CHILD_PIDS_SIZE 16
char* ApplicationPath = NULL;
char* CurrentDirectory = NULL;
pid_t* childpids = NULL;
unsigned int number_of_pids = 0;
unsigned int number_of_used_pids = 0;
void kill_pid(pid_t pid);
char str[SHELL_MAX_INPUT_SIZE];
void GetApplicationPath(void);
void shell(void);
void add_pid(pid_t pid);
void pause_pid(pid_t pid);
void cont_pid(pid_t pid);
void query_pid(pid_t pid);
char * uitoa(unsigned int value, char * str, int base );
void cleanup(void);
int main(int argc, const char* argv[]){
	GetApplicationPath();
	printf("REX Server Environment -- Version: %s\n", SRC_DEV_VER);
	printf("You have launched the REX Server environment, type 'help' for help.\n");
	(void)argc; (void)argv;
	CurrentDirectory = malloc(strlen(ApplicationPath)+1);
	strcpy(CurrentDirectory, ApplicationPath);
	childpids = malloc(DEFAULT_CHILD_PIDS_SIZE*sizeof(pid_t));
	memset(childpids, 0, DEFAULT_CHILD_PIDS_SIZE);
	number_of_pids = DEFAULT_CHILD_PIDS_SIZE;
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
			printf("R3X Server Environment, Version %s\n", SRC_DEV_VER);
			printf("quit: Quits this shell\n");
			printf("run <application path>: Run application through VM.\n");
			printf("kill <pid>: kill a client\n");
			printf("pause <pid>: Pause a client\n");
			printf("cont <pid>: Continue a client\n");
			printf("status <pid>: Send a SIGUSR signal to client, and print useful info\n");
		} else if(strncmp(input, "quit\n", 5)==0){
			cleanup();
			free(childpids);
			exit(EXIT_SUCCESS);
		} else if(strncmp(input, "kill", 4) == 0) {
			(void)strtok(input, " ");
			char* prog = (strtok(NULL, " "));
			if(prog != NULL){
				pid_t pid = atoi(prog);
				kill_pid(pid);
			}
		} else if(strncmp(input, "pause", 5) == 0) {
			(void)strtok(input, " ");
			char* prog = (strtok(NULL, " "));
			if(prog != NULL){
				pid_t pid = atoi(prog);
				pause_pid(pid);
			}
		} else if(strncmp(input, "cont", 4) == 0) {
			(void)strtok(input, " ");
			char* prog = (strtok(NULL, " "));
			if(prog != NULL){
				pid_t pid = atoi(prog);
				cont_pid(pid);
			}
		} else if(strncmp(input, "status", 6) == 0) {
			(void)strtok(input, " ");
			char* prog = (strtok(NULL, " "));
			if(prog != NULL){
				pid_t pid = atoi(prog);
				query_pid(pid);
			}
		} else if(strncmp(input, "run", 3)==0) {
				(void)strtok(input, " ");
				char* program = strtok(NULL, " ");
				if(program == NULL){
					printf("Error program unspecified\n");
				} else {
					for(size_t i = 0; i < strlen(program); i++){
						if(program[i]=='\n'){
							program[i] = 0;
						}
					}
					char** arguments = malloc(4*sizeof(char*));
					arguments[0] = "-exe";
					arguments[1] = program;
					arguments[2] = "-s";
					arguments[3] = NULL;
					pid_t pid = fork();
					if(pid == 0){
						int errcode = execve("./rxvm", arguments, environ);
						if(errcode==-1){
							perror("execve: ");
							printf("Unable to run ./rxvm! Make sure your server executable resides the VMs directory!\n");
						}
						_exit(0);
					} else if(pid == -1) {
						printf("Server fatal error: Unable to start child process!\n");
						exit(EXIT_FAILURE);
					} else {
						printf("Client PID: %d\n", (pid_t)pid);
						add_pid(pid);
						printf("Continue Execution? [y/N]: ");
						pause_pid(pid);
						char input = getchar();
						if(input != 'N' && input != 'n'){
							cont_pid(pid);
						}
					}
				}
		} else if(strncmp(input, "\n", 1)==0){

		} else {
			printf("Try again!\n");
		}
	}
}
void kill_pid(pid_t pid){
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i] == pid){
			kill(childpids[i], SIGKILL);
			childpids[i] = 0;
			printf("Killed Client\n");
			return;
		}
	}
	printf("Invalid Client PID\n");
}
void cleanup(void){
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i]!= 0){
			kill(childpids[i], SIGKILL);
			printf("Killing Client: %d\n", (int)childpids[i]);
		}
	}
}
void add_pid(pid_t pid){
	if(number_of_used_pids >= number_of_pids-1){
		pid_t* newbuf = realloc(childpids, sizeof(pid_t)*(number_of_pids+16));
		if(newbuf == NULL){
			printf("Unable to allocate buffers for pids\n");
			exit(EXIT_FAILURE);
		}
		number_of_pids += 16;
	}
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i]==0){
			childpids[i] = pid;
			return;
		}
	}
}
void pause_pid(pid_t pid){
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i] == pid){
			kill(childpids[i], SIGSTOP);
			printf("Client on hold\n");
			return;
		}
	}
	printf("Invalid Client PID\n");
}
void cont_pid(pid_t pid){
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i] == pid){
			kill(childpids[i], SIGCONT);
			printf("Continuing execution of client\n");
			return;
		}
	}
	printf("Invalid Client PID\n");
}
void query_pid(pid_t pid){
	for(unsigned int i = 0; i < number_of_pids; i++){
		if(childpids[i] == pid){
			printf("Sending message to client\n");
			kill(childpids[i], SIGUSR1);
			return;
		}
	}
	printf("Invalid Client PID\n");
}