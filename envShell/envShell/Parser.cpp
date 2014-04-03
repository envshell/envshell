#include "Parser.h"
#include "Scanner.h"

Parser::Parser(string commandString) {
	myCommandString = commandString;
}

string Parser::getCommandString(){
	return myCommandString;
}

void Parser::parse(){
	Scanner s;
	myTokens = s.scan(myCommandString);
	setValues();
}

//set token variables to the private command, args, infile, outfile values
void Parser::setValues(){
	myCommand = myTokens[0]->getValue();
	//store arguments until hit "<" or EOL
	int i = 1;
	while(myTokens[i]->getValue() != "<" && (myTokens[i]->getValue() != "eol" && myTokens[i]->getType() == "eol")){
		myArguments.push_back(myTokens[i]->getValue());
		i++;
	}
	//check to see if there is an input file
	if(myTokens[i]->getValue() == "<"){
		myInfile = myTokens[i++]->getValue();
	}
	//check to see if there is an output file
	if(myTokens[i]->getValue() == ">"){
		myOutfile = myTokens[i++]->getValue();
	}
	runProgram();
}


void Parser::runProgram(){
	//built-in commands
	if(myCommand == "%"){
		//the argument will be the comment
		myComment = myArguments[0];
	}else if(myCommand == "prompt"){
		//set the shell prompt to the prompt argument
		myNewShellPrompt = myArguments[0];
	}else if(myCommand == "setenv"){
		//set the environment variable to the value contained in the string
		myEnvironmentVariable = myArguments[0];
		myEnvironmentVariableString = myArguments[1];
	}else if(myCommand == "unsetenv"){
		//remove this environment variable from the list passed to applications
		myEnvironmentVariable = myArguments[0];
	}else if(myCommand == "listenv"){
		//prints the list of environment variables and their values
	}else if(myCommand == "setdir"){
		//set shell's concept of current directory to directory_name (See getwd(3) and chdir(2))
		myDirectoryName = myArguments[0];
	}else if(myCommand == "bye"){
		//exit the shell program
	}else if(myCommand == "^D"){
		//exit the shell program
	}else{
		//the command is a user-program command, need to use fork() and wait() until the child finishes

	}








}