#include "Parser.h"
#include "Scanner.h"
#include "EnvVar.h"

Parser::Parser(string commandString) {
	myCommandString = commandString;
}

string Parser::getCommandString(){
	return myCommandString;
}

void Parser::parse(){
	Scanner s;
	try{
	myTokens = s.scan(myCommandString);
	}catch(int i){
	cout << "Exception Number: " << i << endl;
	}
	setValues();
}

//set token variables to the private command, args, infile, outfile values
void Parser::setValues(){
	myCommand = myTokens[0]->getValue();
	//store arguments until hit "<" or EOL
	int i = 1;
	while (myTokens[i]->getValue() != "<" && myTokens[i]->getValue() != ">" && (myTokens[i]->getValue() != "EOL" && myTokens[i]->getType() != "end-of-line")){
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
		EnvVar* element = new EnvVar(myArguments[0], myArguments[1]);
		//element.setName = myArguments[0];
		//element.setValue = myArguments[1];
		environmentVariables.push_back(element);
		
	}else if(myCommand == "unsetenv"){
		//remove this environment variable from the list passed to applications
		EnvVar* element = new EnvVar(myArguments[0], myArguments[1]);
		//find element in the environmentVariable list and delete

	}else if(myCommand == "listenv"){
		//prints the list of environment variables and their values
		for(int i = 0; i < environmentVariables.size(); i++){
			printf("Environment Variable: %s       Variable Value: %s", environmentVariables[i]->getName(), environmentVariables[i]->getValue());
		}
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