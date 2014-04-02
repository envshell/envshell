#ifndef PARSER_H
#define PARSER_H


#include<string>
#include<vector>
#include"Token.h"
using namespace std;

class Parser{
public:
	Parser(string commandString);
	void parse();

	string getCommandString();

	void setValues();

	void runProgram();

private:
	//Parts of input 
	string myCommandString;
	vector<Token*> myTokens;
	string myCommand;
	vector<string> myArguments;
	string myInfile;
	string myOutfile;


	string myComment;
	string myNewShellPrompt;
	string myEnvironmentVariable;
	string myEnvironmentVariableString;
	string myDirectoryName;


};

#endif