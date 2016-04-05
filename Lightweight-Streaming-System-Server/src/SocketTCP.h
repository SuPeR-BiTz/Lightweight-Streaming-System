#ifndef SOCKETTCP_H_
#define SOCKETTCP_H_

#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_PEERS	3
#define BUFFER_SIZE	512

using namespace std;

class SocketTCP {
public:
	SocketTCP(int);
	void* listenTCP(void);
	void sendTCP(void*, size_t);
	virtual ~SocketTCP(void);
private:
	int server_ID;
	int client_ID;
	short connectedFlag;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server;
	struct sockaddr_in client;
};

#endif /* SOCKETTCP_H_ */
