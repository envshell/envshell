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
	~Parser();

	bool parse(string & prompt);
	string getCommandString();
	bool setValues(string & prompt);
	bool runProgram(string & prompt);

private:
	/*
	Need to call this whenever we want a char** to pass to exec. This will set a member variable. DO NOT call
	the member variable directly because it may not be updated. The only reason we leep the member variable is so
	that we can hold the pointer to free the memory created in the dynamic array allocation.
	*/
	char ** envVarConvert();	//Private method to convert our environmentVariables vector to a char** that can be passed to exec methods

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
	
	//Only hold this so we can free it, see the explanation of envVarConvert()
	char ** envVarArray;
	int envVarRows;
};

#endif