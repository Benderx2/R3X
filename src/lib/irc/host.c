#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "virtual-machine.h"
r3x_cpu_t* CPU = NULL;

int get_ip_from_hostname(char * hostname, char* ip);

void Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
}

uint32_t host_connect(/*char *server, unsigned int port*/) {
	char* server_name = GetLinearAddress(CPU, GetArgument(CPU, 1,2));
	char* server = malloc(80);
	get_ip_from_hostname(server_name, server);
	printf("Server Name: %s\n", server_name);
	unsigned int port = GetArgument(CPU, 2,2);
	printf("Connecting to server and port: %s, %u\n", server, port);
	int sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("host_connect FAILED\n");
        return (uint32_t)-1;
    }

    if (inet_pton(AF_INET, server, &servaddr.sin_addr) <= 0) {
		printf("host_connect FAILED\n");
        return (uint32_t)-1;
    }
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) {
		printf("host_connect FAILED\n");
        return (uint32_t)-1;
    }
    return sockfd;
}
uint32_t host_send(void) {
	int sockfd = (int)GetArgument(CPU,1,2);
	char* arg2 = (char*)GetLinearAddress(CPU, GetArgument(CPU,2,2));
	char* out = malloc(strlen(arg2+2));
	memset(out, 0, strlen(arg2+2));
	strcpy(out, arg2);
	strcat(out, "\n");
	//printf("Sending message: %s to socket: %d\n", out, sockfd);
    int retval = (uint32_t)send(sockfd, out, strlen(out), 0);
	return retval;
}
uint32_t host_receive(void) {
	int sockfd = (int)GetArgument(CPU, 1,3);
	unsigned int length = (unsigned int)GetArgument(CPU, 2,3);
	char* buf = GetLinearAddress(CPU, GetArgument(CPU, 3,3));
	memset(buf, 0, length);
	int n = 0;
	while(n == 0) {
		n = read(sockfd, buf, length);
	}
	return n;
}
uint32_t host_receive_no_wait(void) {
	int sockfd = (int)GetArgument(CPU, 1,3);
	unsigned int length = (unsigned int)GetArgument(CPU, 2,3);
	char* buf = GetLinearAddress(CPU, GetArgument(CPU, 3,3));
	memset(buf, 0, length);
	int n = read(sockfd, buf, length);
	return n;
}
int get_ip_from_hostname(char* hostname, char* ip) 
{  
	struct hostent *host_entry;     
	struct in_addr **address_list;     
	int i;     
	if ((host_entry = gethostbyname(hostname)) == NULL) { 
		herror("gethostbyname");         
		return 1;
	}     
	address_list = (struct in_addr **)host_entry->h_addr_list;
	for(i = 0; address_list[i] != NULL; i++)
	{   
		strcpy(ip, inet_ntoa(*address_list[i]));
		return 0;
	}
	return 1;
}
