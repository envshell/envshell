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

	void doSomething();

private:
	string myString;
	vector<Token> myTokens;
	string myCommand;
	vector<string> myArguments;
	string myInfile;
	string myOutfile;


};

#endif