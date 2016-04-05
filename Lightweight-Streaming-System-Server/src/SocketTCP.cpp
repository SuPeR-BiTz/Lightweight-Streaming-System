#include "SocketTCP.h"

SocketTCP::SocketTCP(int serverPort) {
	/* set server socket ID's */
	server_ID = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* check if server socket ID is valid */
	if(server_ID < 0) {
		fprintf(stderr, "\nERROR: Failed to create TCP socket\n       %s",strerror(errno));
	}

	/* clear socket structure */
	memset(&server, 0, sizeof(server));

	/* construct structure */
	server.sin_family = AF_INET;				/* get and set server's IPv4 */
	server.sin_addr.s_addr = htonl(INADDR_ANY);	/* allow server to accept anyone */
	server.sin_port = htons(serverPort);		/* set server port */

	/* bind socket and check if successful */
	if(bind(server_ID, (struct sockaddr *) &server, sizeof(server)) < 0) {
		fprintf(stderr, "\nERROR: Failed to bind TCP socket\n       %s",strerror(errno));
	}

	/* mark socket connected */
	connectedFlag = 0;
}

SocketTCP::~SocketTCP() {
	close(server_ID);
}

void* SocketTCP::listenTCP(void) {
	/* set up connection */
	if(connectedFlag != 1) {
		/* start listening */
		if(listen(server_ID, MAX_PEERS) < 0) {
			fprintf(stderr, "\nERROR: Failed to listen to buffer socket\n       %s",strerror(errno));
		}

		/* wait for client connection */
		unsigned int clientSize = sizeof(client);
		if((client_ID = accept(server_ID, (struct sockaddr *) &client, &clientSize)) >= 0) {
			connectedFlag = 1;
		}
	}

	/* get data from socked and return it */
	recv(client_ID, buffer, BUFFER_SIZE, 0);
	fprintf(stderr, "%s", buffer);
	return buffer;
}

void SocketTCP::sendTCP(void* data, size_t dataSize) {
	assert(client.sin_port >= 0 && client.sin_port <= 65535);	/* ensure client structure was set up by listenTCP() */
	if(send(client_ID, data, dataSize, 0) != (unsigned int)dataSize) {
			fprintf(stderr, "\nERROR: Failed to send from input socket\n       %s",strerror(errno));
		}
}
