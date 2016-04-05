#include "SocketTCP.h"

SocketTCP::SocketTCP(char* serverIP, int serverPort) {
	/* set server socket ID's */
	server_ID = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* check if server socket ID is valid */
	if(server_ID < 0) {
		fprintf(stderr, "\nERROR: Failed to create TCP socket\n");
	}

	/* clear socket structure */
	memset(&server, 0, sizeof(server));

	/* construct structure */
	server.sin_family = AF_INET;					/* use IP */
	server.sin_addr.s_addr =  inet_addr(serverIP);	/* set server IP */
	server.sin_port = htons(serverPort);			/* set server port */

	/* connect to server */
	if (connect(server_ID, (struct sockaddr *) &server, sizeof(server)) < 0) {
		fprintf(stderr, "\nERROR: Failed to connect to server\n       %s",strerror(errno));

	}
	/* mark socket connected */
	else {
		connectedFlag = 0;
	}
}

SocketTCP::~SocketTCP() {
	close(server_ID);
}

void SocketTCP::sendTCP(void* data, size_t dataSize) {
	if(send(server_ID, data, dataSize, 0) != (unsigned int)dataSize) {
			fprintf(stderr, "\nERROR: Failed to send from socket\n       %s",strerror(errno));
		}
}

void* SocketTCP::listenTCP(void) {
	/* get data from socket and return it */
	if(recv(server_ID, buffer, BUFFER_SIZE, 0) < 1) {
		fprintf(stderr, "\nERROR: Failed to receive data\n       %s",strerror(errno));
	}
	return buffer;
}
