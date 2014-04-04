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

	bool parse(string & prompt, vector<EnvVar*> & inEnvVars);
	string getCommandString();
	bool setValues(string & prompt, vector<EnvVar*> & inEnvVars);
	bool runProgram(string & prompt, vector<EnvVar*> & inEnvVars);

private:
	char ** envVarConvert(int rows, vector<EnvVar*> & inEnvVars);	//Private method to convert our environmentVariables vector to a char** that can be passed to exec methods
	char ** argConvert(int rows);		//Same for arguments

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
	//vector<EnvVar *> environmentVariables;
	
	/*
	//Only hold this so we can free it, see the explanation of envVarConvert()
	char ** envVarArray;
	int envVarRows;
	*/
};

#endif