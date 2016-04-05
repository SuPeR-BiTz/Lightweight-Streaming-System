#ifndef SOCKETTCP_H_
#define SOCKETTCP_H_

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE	512

using namespace std;

class SocketTCP {
public:
	SocketTCP(char*, int);
	void sendTCP(void*, size_t);
	void* listenTCP(void);
	virtual ~SocketTCP(void);
private:
	int server_ID;
	short connectedFlag;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server;
};

#endif /* SOCKETTCP_H_ */
