#include "SV_UserInput.h"

/**
 *  This constructor initializes port numbers using the stored config
 *  file and sets up the necessary requirements for listening for
 *  commands that come from the client.
 *  @author Dan McCarten
 */
SV_UserInput::SV_UserInput() {
	prepareConfigFile();
	cmdSocket = new SocketTCP(cmdSocketPort);
}

/**
 *	Deallocates memory held by this object
 */
SV_UserInput::~SV_UserInput() {
	delete cmdSocket;
}

/**
 *	Listens, using a TCP socket, to commands that come from the client.
 *	IMPORTANT: Call this function in its own thread, because it stops
 *	execution to listen to the socket.
 */
char* SV_UserInput::listenFromClient() {
	char* messageFromClient = NULL;
	while (messageFromClient == NULL) {
		messageFromClient = (char*) cmdSocket->listenTCP();
	}

	if (strstr(messageFromClient, "PortNo:") != NULL) {
		char dupe[18];
		//Copy string so the original can be sent unchanged
		strcpy(dupe, messageFromClient);
		char* parse1 = strtok(dupe, ":");
		//Covers the case where no port number is given, just "SV_XXXPortNo:"
		if ((parse1 = strtok(NULL, ":")) == NULL) {
			parse1 = (char*) "-1";
		}

		std::fstream fileStream;
		std::string line;

		fileStream.open(
				"portConfig.dat", std::fstream::in | std::fstream::out);
		fileStream.seekg(0);

		if (strstr(messageFromClient, "SV_ACKPortNo:") != NULL) {
			std::getline(fileStream, line);
			fileStream.seekp(fileStream.tellg() + 10L);
			fileStream << std::string(parse1);
		}
		else if (strstr(messageFromClient, "SV_cmdPortNo:") != NULL) {
			std::getline(fileStream, line);
			std::getline(fileStream, line);
			fileStream.seekp(fileStream.tellg() + 10L);
			fileStream << std::string(parse1);

		}
		else if (strstr(messageFromClient, "SV_streamPortNo:") != NULL) {
			fileStream.seekp((long)10);
			fileStream << std::string(parse1);
		}
		fileStream.close();
	}

	if (strcmp(messageFromClient, "SV_Quit") == 0) {
		printf("Quitting Server Program...\n");
		exit(0);
	}
	return messageFromClient;
}

/**
 * 	Loads the parameters from the port config file into the program. If
 * 	the file does not exist or the parameters are improperly formatted
 * 	or invalid, default port numbers are used.
 */
void SV_UserInput::prepareConfigFile() {
	bool validConfigFile = true;
	struct stat buffer;
	char filename[] = "portConfig.dat";

#ifndef VLCSIM
	int portNums[3]; //VLC Port not in config file
#endif

#ifdef VLCSIM
	int portNums[4];
#endif

	if (stat(filename, &buffer) != 0) { //File failed to open
		validConfigFile = false;

		//Use default ports and create config file
		std::ofstream configFileOutputStream;
		configFileOutputStream.open("portConfig.dat");
		configFileOutputStream << "UDP_Port: 51346\n";
		configFileOutputStream << "ACK_Port: 51348\n";
		configFileOutputStream << "CMD_Port: 51349";
		streamSocketPort = 51346; //RTP Even port number
		ackSocketPort = 51348;
		cmdSocketPort = 51349;

#ifdef VLCSIM
		configFileOutputStream << "\nVLC_Port: 51350";
		vlcSocketPort = 51350;
#endif
		configFileOutputStream.close();

	}
	else { //File was found
		std::ifstream configFileStream;
		configFileStream.open("portConfig.dat");
		int i = 0;
		//While the config file is valid AND
		//The end of file hasn't been reached
		while (validConfigFile && !configFileStream.eof()) {
			std::string line;
			getline(configFileStream, line);

			//Get the index of the first valid digit
			int numPos = line.find_first_of("12345");

			//If number not found
			if (numPos == -1) {
				validConfigFile = false;
				//Else number found
			}
			else {
				std::string portNumStr = line.substr(numPos);
				//Extract number from read string
				portNums[i] = atoi(portNumStr.c_str());
				i++;
			}
		}
		// All numbers successfully read into portNums array
		if (validConfigFile) {

			//Range checking the port numbers
			if (portNums[0] > 0 && portNums[0] < 65536 && portNums[1] > 0
					&& portNums[1] < 65536 && portNums[2] > 0
					&& portNums[2] < 65536) {

				streamSocketPort = portNums[0];
				ackSocketPort = portNums[1];
				cmdSocketPort = portNums[2];
			}
			else {
				/*ONE OR MORE PORT NUMBERS ARE OUT OF RANGE*/
				printf(
						"\nOne or more port numbers are out of range 0 - 65535.\n");
			}

#ifdef VLCSIM
			if (portNums[3] > 0 && portNums[3] < 65536) {
				vlcSocketPort = portNums[3];
			}
			else {
				printf("\nVLCSIM port number is out of range 0 - 65535.\n");
			}
#endif

		}
		else {
			printf(
					"Error on config file line: %d\n Fix issues or delete file and run again to create default file.\nUndefined behavior will occur if you continue.\n",
					i + 1);
		}
		configFileStream.close();
	}
}

/**
 * Returns the socket port used to transmit RTP packets via UDP
 * transport protocol
 * @return UDP Socket Port used for transmitting RTP
 */
unsigned short SV_UserInput::getStreamPort() {
	return streamSocketPort;
}

/**
 * Returns the socket port used to transmit TCP ack messages for
 * received RTP packets.
 * @return TCP Socket port used to receive ack messages
 */
unsigned short SV_UserInput::getAckPort() {
	return ackSocketPort;
}
