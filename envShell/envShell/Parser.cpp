#include "Parser.h"
#include "Scanner.h"

Parser::Parser(string commandString) {
	myString = commandString;
}

string Parser::getCommandString(){
	return myString;
}

void Parser::parse(){
	Scanner s;
	myTokens = s.scan(myString);
	doSomething();
}

void Parser::doSomething(){
	myCommand = myTokens[0].getValue;
	//store arguments until hit "<" or EOL
	int i = 1;
	while(myTokens[i].getValue() != "<" && myTokens[i].getValue() != "EOL"){
		myArguments.push_back(myTokens[i].getValue());
		i++;
	}
	//check to see if there is an input file
	if(myTokens[i].getValue() == "<"){
		myInfile = myTokens[i++].getValue();
	}
	//check to see if there is an output file
	if(myTokens[i].getValue() == ">"){
		myOutfile = myTokens[i++].getValue();
	}

}