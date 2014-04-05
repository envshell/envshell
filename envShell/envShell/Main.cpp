#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "Scanner.h"
#include "Parser.h"

//Uncomment this
#include <unistd.h>
using namespace std;

int main() {

	string prompt = "envsh > ";
	bool loop = true;
	vector<EnvVar*> envVars;
	envVars.push_back(new EnvVar("test", "hopefully this passes"));
	
	while (loop){
		printf("%s", prompt.c_str());

		printf("Enter a command and any arguments you may have (separated by a space): ");
		char cmd[256];
		//get user input
		fgets(cmd, 256, stdin);
		cmd[strlen(cmd) - 1] = '\0';	/* this removes the \n */
		string command = cmd;

		/*
		string command;
		command = "/usr/bin/wc -l paper.txt";
		*/

		//Envar testers
		/*
		if (envVars.size() == 1) {
			command = "setenv var1 thisisntfun";
		}
		else if (envVars.size() == 2) {
			command = "setenv var2 thisisntfun";
		}
		else if (envVars.size() == 3) {
			command = "unsetenv var1 thisisntfun";
		}		
		*/

		Parser* p = new Parser(command);
		try {
			loop = p->parse(prompt, envVars);
		}
		catch (int i) {
			cout << "Exception number " << i << " thrown. See the documentation for explanation of exception number." << endl;
		}
	}

	cin.ignore(200, '\n');

	return 0;
}