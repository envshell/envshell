#ifndef PARSER_H
#define PARSER_H

#include<string>
#include<vector>
#include"Token.h"
#include "EnvVar.h"
#include<iostream>
using namespace std;

class Parser{
public:
	Parser(string commandString);
	bool parse(string & prompt);

	string getCommandString();

	bool setValues(string & prompt);

	bool runProgram(string & prompt);

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
	vector<EnvVar *> environmentVariables;


};

#endif