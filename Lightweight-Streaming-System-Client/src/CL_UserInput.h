#ifndef _CL_USERINPUT_H_
#define _CL_USERINPUT_H_

#include "SocketTCP.h"
#include <iostream>
#include <string>

class CL_UserInput {
	public:
		CL_UserInput();
		~CL_UserInput();

		void getTerminalInput();
		std::string getLastTerminalInput();

	private:
		std::string lastTerminalInput;
		SocketTCP * inputSocket;
		void parseTerminalInput(char*);
		void parsePortChangeCommand(char*);
};


#endif /*_CL_USERINPUT_H_*/
