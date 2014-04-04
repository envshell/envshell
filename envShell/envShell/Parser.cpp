#include "Parser.h"
#include "Scanner.h"
#include "EnvVar.h"
//#include <unistd.h>

Parser::Parser(string commandString) {
	myCommandString = commandString;
	envVarRows = -1;	//To indicate it has not been set yet
}

Parser::~Parser() {
	//Free the memory of the envVarArray
	for (int i = 0; i < envVarRows; i++) {
		delete[] envVarArray[i];
	}

	delete[] envVarArray;

	//Free vector of EnvVars
	for (int i = 0; i < environmentVariables.size(); i++) {
		delete environmentVariables[i];
	}
}

string Parser::getCommandString(){
	return myCommandString;
}

bool Parser::parse(string & prompt){
	Scanner s;
	try{
	myTokens = s.scan(myCommandString);
	}catch(int i){
	cout << "Exception Number: " << i << endl;
	}
	return setValues(prompt);
}

//set token variables to the private command, args, infile, outfile values
bool Parser::setValues(string & prompt){
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

	return runProgram(prompt);
}


bool Parser::runProgram(string & prompt){
	//built-in commands
	if(myCommand == "%"){
		//the argument will be the comment
		myComment = myArguments[0];
		//printf("% %s", myComment);
		cout << "% " << myComment << endl;
	}else if(myCommand == "prompt"){
		//set the shell prompt to the prompt argument
		myNewShellPrompt = myArguments[0];
		prompt = myNewShellPrompt;
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
		//note: uncomment #include <unistd.h>
		//chdir changes the directory to the given path. 
		//But chdir takes  a const char* and myDirectoryName is a string. 
		if(execve("/bin/chdir", myDirectoryName.c_str(), envVarConvert()) != 0){		//correct behavior returns a 0, incorrect returns -1
			//something went wrong
			printf("Path Error, did not change directory.");
		}
	}else if(myCommand == "bye"){
		//exit the shell program
		return false;
	}else if(myCommand == "^D"){
		//exit the shell program
		return false;
	}else{
		//the command is a user-program command, need to use fork() and wait() until the child finishes
		//execv(const char *path, char *const argv[]); 
		pid_t kidpid = fork();
		if(kidpid < 0){
			printf("Internal error, cannot fork.");
		}else if(kidpid==0){
			//This is the child
			execv(myCommand, myArguments);
			//should never reach the next line
			printf("Error in the command.");
		}
		else{
			if (waitpid(kidpid, 0, 0) < 0){
				printf("Cannot wait for child.");
			}
		}
	}

	return true;

}

/*
	Need to call this whenever we want a char** to pass to exec. This will set a member variable. DO NOT call
	the member variable directly because it may not be updated. The only reason we leep the member variable is so
	that we can hold the pointer to free the memory created in the dynamic array allocation.
*/
char ** Parser::envVarConvert() {
	//Free the memory of the current envVarArray if already set
	if (envVarRows != -1) {
		for (int i = 0; i < envVarRows; i++) {
			delete[] envVarArray[i];
		}

		delete[] envVarArray;
	}

	char ** returnArray;
	envVarRows = environmentVariables.size();
	returnArray = new char *[envVarRows];	//initialize number of variables
	for (int i = 0; i < envVarRows; i++) {
		returnArray[i] = new char[256];			//Each var has room for 256 chars
		char * copy = new char[256];
		strcpy(copy, environmentVariables[i]->getValue().c_str());	//Workaround to convert a const char * to a char *
		returnArray[i] = copy;

		delete[] copy;
		int foo = 5;
	}

	envVarArray = returnArray;
	return returnArray;
}