#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "Scanner.h"
#include "Parser.h"
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
		if (envVars.size() > 1) {
			string command = "setenv var1 thisisntfun";
		}
		else {
			string command = "unsetenv var1 thisisntfun";
		}
		cout << command;
		cin.ignore(200, '\n');
		Parser* p = new Parser(command);
		loop = p->parse(prompt, envVars);

		//Print out the environment variables
		for (int i = 0; i < envVars.size(); i++) {
			cout << "Environment Variable " << i << ":   Name:" << envVars[i]->getName() << "    Value: " << envVars[i]->getValue() << endl;
		}
	}

	cin.ignore(200, '\n');

	return 0;
}