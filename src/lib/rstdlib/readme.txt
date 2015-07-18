this file contains the socket implementation for r3x and an example in t++.

functions:

@host_connect(server_name, port) -> Connects to the server with the specified port and returns the socket descriptor.

@host_send(sockfd, string) -> sends a null terminated "string" to sockfd with a newline '\n'

@host_receive(sockfd, length, buffer) -> Reads from socket sockfd, to buffer with specified length. This function will
!WAIT! until data is received. returns the number of bytes read.

@host_receive_no_wait(sockfd, length, buffer) -> Reads from socket sockfd, to buffer with specified length. This function does not wait for data to be received. if there is no data received, it returns 0, else it returns the
number of bytes read.
