#ifndef _SV_USERINPUT_H_
#define _SV_USERINPUT_H_

#include "SocketTCP.h"
#include <fstream>
#include <sys/stat.h>

/**
 * SV_UserInput.h
 * @author Dan McCarten
 *
 *  Overview:
 * 		This class loads a config file consisting of port numbers
 * 	and also listens for commands from a TCP socket.
 */

// Comment out this line for final build and VLC simulation
// aspects of this class will be ignored
#define VLCSIM

class SV_UserInput {
	public:
		SV_UserInput();
		~SV_UserInput();

		char* listenFromClient();

		unsigned short getStreamPort();
		unsigned short getAckPort();

	private:
		unsigned short streamSocketPort, ackSocketPort, cmdSocketPort;
		SocketTCP* cmdSocket;

#ifdef VLCSIM
		unsigned short vlcSocketPort;
#endif


		void prepareConfigFile();
};
#endif /*_SV_USERINPUT_H_*/
