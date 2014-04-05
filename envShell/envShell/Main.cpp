#include <string>
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

		//get user input
		//fgets(char* str, int n, FILE* stream)
		//Fgets(command, 256, c.in)

		string command;
		command = "/usr/bin/wc -l paper.txt";

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

		cout << command;
		cin.ignore(200, '\n');
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