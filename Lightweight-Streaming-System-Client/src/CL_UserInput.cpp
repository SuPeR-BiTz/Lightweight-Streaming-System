#include "CL_UserInput.h"

/**
 * Default constructor for CL_UserInput
 */
CL_UserInput::CL_UserInput() {
	char* IP = (char*)"127.0.0.1";
	int portNo = 51349;
	inputSocket = new SocketTCP(IP, portNo);
	lastTerminalInput = "";
}

/**
 * Default destructor for CL_UserInput
 */
CL_UserInput::~CL_UserInput() {
	delete inputSocket;
}

std::string CL_UserInput::getLastTerminalInput() {
	return lastTerminalInput;
}


/**
 * Parses the input from the terminal appropriately. If it is a
 * client command, it will be processed locally. If it is a server
 * command, it will be sent out to the server via TCP Socket.
 * @param terminalInput - The command inputted via terminal
 */
void CL_UserInput::parseTerminalInput(char* terminalInput) {

	/*
	 * 	SERVER COMMANDS
	 */
	char* serverParse = strstr(terminalInput, "SV_"); //It's an SV Command
	if(serverParse != NULL) { //Potentially server command
		if(strcmp(terminalInput, "SV_Stop") == 0) {
			inputSocket->sendTCP(terminalInput, strlen(terminalInput)+1);
		}
		else if(strcmp(terminalInput, "SV_Start") == 0) {
			inputSocket->sendTCP(terminalInput, strlen(terminalInput)+1);
		}
		else if(strstr(terminalInput, "SV_ACKPortNo:") != NULL) {
			parsePortChangeCommand(terminalInput);
		}
		else if(strstr(terminalInput, "SV_streamPortNo:") != NULL) {
			parsePortChangeCommand(terminalInput);
		}
		else if(strstr(terminalInput, "SV_cmdPortNo:") != NULL) {
			parsePortChangeCommand(terminalInput);
		}
		else if(strstr(terminalInput, "SV_Quit") != NULL) {
			inputSocket->sendTCP(terminalInput, strlen(terminalInput)+1);
		}
		else {
			printf("Invalid Server Command. Type \"help\" for command list.\n");
		}

	}

	/*
	 * 	CLIENT COMMANDS
	 */
	char* clientParse = strstr(terminalInput, "CL_"); //It's a CL Command
	if(clientParse != NULL) { //Potentially client command
		lastTerminalInput = std::string(terminalInput);
		if(terminalInput == strstr(terminalInput, "CL_Stop")) {
		}
		else if(terminalInput == strstr(terminalInput, "CL_Start")) {
		}
		else {
			printf("Invalid Client Command. Type \"help\" for command list.\n");
		}
	}

	if(strcmp(terminalInput, "help") == 0){
		printf("\n---Server Commands---\n");
		printf("  %-21s - Tell server to start streaming\n", "SV_Start");
		printf("  %-21s - Tell server to stop streaming\n", "SV_Stop");
		printf("  %-21s - Tell server to terminate\n", "SV_Quit");
		printf("  %-21s - Change server's ack port to ##### on restart\n", "SV_ACKPortNo:#####");
		printf("  %-21s - Change server's command receiving port to ##### on restart\n", "SV_cmdPortNo:#####");
		printf("  %-21s - Change server's RTL Streaming port to ##### on restart\n", "SV_streamPortNo:#####");

		printf("\n---Client Commands---\n");
		printf("  %-21s - Sends a SV_QUIT command then closes this client\n\n", "CL_Quit");
	}

	//Neither a client command or a server command
	if(serverParse == NULL && clientParse == NULL) {
		printf("Invalid Command. Type \"help\" for a list of commands.\n");
	}
}

/**
 * Further parses port change commands because they need to be range checked
 * before being sent to the server, because the server side will not do these
 * checks. If valid, this command will also send the command to the server.
 * @param portChangeCommand - The entire SV_XXXPortNo:##### command to parse
 */
void CL_UserInput::parsePortChangeCommand(char* portChangeCommand) {
	char dupe[18];
	//Copy string so the original can be sent unchanged
	strcpy(dupe,portChangeCommand);
	char* parse1 = strtok(dupe, ":");
	//Covers the case where no port number is given, just "SV_XXXPortNo:"
	if((parse1 = strtok(NULL, ":")) == NULL) {
		parse1 = (char*)"-1";
	}
	int newPortNo = atoi(parse1);
	//Check range of port inputted
	if(newPortNo > 0 && newPortNo < 65536) {
		inputSocket->sendTCP(portChangeCommand, strlen(portChangeCommand)+1);
	}
	else {
		printf("Invalid Port Number: Must be 0 - 65535\n");
	}
}

/**
 * Listens for input from terminal. IMPORTANT! Run this function in
 * its own thread, because the cin call will halt any thread it's a
 * part of.
 */
void CL_UserInput::getTerminalInput() {
	std::string inputFromTerminal;
	std::cout << "> ";
	std::getline(std::cin, inputFromTerminal);

	if(inputFromTerminal.compare("CL_Quit") == 0) {
		//TODO:Take this out on final program so you can close the client only
		inputSocket->sendTCP((char*)"SV_Quit", 8);

		printf("Quitting Client Program...\n");
		this->~CL_UserInput();
		exit(0);
	}
	//React to terminal input
	parseTerminalInput((char*)inputFromTerminal.c_str());
}
