//To run the program with a test file with commands, run "make" then type "./envshell testfile.txt"
//the test file should be passed as an argument

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include "Scanner.h"
#include "Parser.h"

//Uncomment this
#include <unistd.h>
using namespace std;

int main(int argc, char * argv[]) {
	string commandFile = "";
	bool fromFile = false;
	if(argc == 2){
		cout << "Command File given" << endl;
		commandFile = argv[1];
		//cout << "Command File: " << commandFile << endl;
		fromFile = true;
	}
	string prompt = "envsh > ";
	bool loop = true;
	vector<EnvVar*> envVars;
	
	if(fromFile){
		string command = "";
		ifstream infile;
		infile.open(argv[1], ios::in);
		getline(infile, command);
		while(!infile.eof() && loop){
			printf("%s %s \n", prompt.c_str(), command.c_str());
			Parser* p = new Parser(command);
			try{
				loop = p->parse(prompt, envVars);
			}catch(int i){
				cout << "Exception number " << i << " thrown. See the documentation for explanation of exception number." << endl;
			}
			getline(infile, command);
		}
		infile.close();

	}else{
		while (loop){

			printf("%s", prompt.c_str());
			char cmd[256];
			//get user input
			fgets(cmd, 256, stdin);
			cmd[strlen(cmd) - 1] = '\0';	/* this removes the \n */
			string command = cmd;

			Parser* p = new Parser(command);
			try {
				loop = p->parse(prompt, envVars);
			}
			catch (int i) {
				cout << "Exception number " << i << " thrown. See the documentation for explanation of exception number." << endl;
			}
		}
	}
	//cin.ignore(200, '\n');

	return 0;
}
