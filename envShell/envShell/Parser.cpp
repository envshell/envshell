#include "Parser.h"
#include "Scanner.h"
#include "EnvVar.h"
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <sys/types.h>

//Uncomment these
#include <sys/wait.h>
#include <unistd.h>

//Takes in the command string and a vector of environment variables by reference. Set our environment variables list with the one passed
Parser::Parser(string commandString) {
	myCommandString = commandString;
	//envVarRows = -1;	//To indicate it has not been set yet
}

Parser::~Parser() {

}

string Parser::getCommandString(){
	return myCommandString;
}

bool Parser::parse(string & prompt, vector<EnvVar*> & inEnvVars){
	//environmentVariables = inEnvVars;

	Scanner s;
	try{
	myTokens = s.scan(myCommandString);
	}catch(int i){
		//Now pass on the exceptions here
		throw i;
	}
	return setValues(prompt, inEnvVars);
}

//set token variables to the private command, args, infile, outfile values
bool Parser::setValues(string & prompt, vector<EnvVar*> & inEnvVars){
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

	return runProgram(prompt, inEnvVars);
}

//return true to continue looping, false to quit the shell
bool Parser::runProgram(string & prompt, vector<EnvVar*> & inEnvVars){
	//built-in commands
	if(myCommand == "%"){
		//the argument will be the comment
		myComment = myArguments[0];
		//printf("%s", myComment.c_str());
		cout << "%" << myComment << endl;
	}else if(myCommand == "prompt"){
		//set the shell prompt to the prompt argument
		myNewShellPrompt = myArguments[0];
		prompt = myNewShellPrompt;
	}else if(myCommand == "setenv"){
		//check if there is a current variable with this name
		if (vectorSearch(myArguments[0], inEnvVars) >= 0) {
			cout << "Variable already exists." << endl;
			return true;
		}

		//set the environment variable to the value contained in the string
		EnvVar* element = new EnvVar(myArguments[0], myArguments[1]);
		inEnvVars.push_back(element);

	}else if(myCommand == "unsetenv"){
		//remove this environment variable from the list passed to applications
		EnvVar* element = new EnvVar(myArguments[0], myArguments[1]);

		//find element in the environmentVariable list and delete
		int index = vectorSearch(myArguments[0], inEnvVars);
		if (index >= 0) {
			inEnvVars.erase(inEnvVars.begin() + index);
			return true;
		}
		else {
			cout << "Variable not found." << endl;
		}
	}else if(myCommand == "listenv"){
		//prints the list of environment variables and their values
		for(int i = 0; i < inEnvVars.size(); i++){
			printf("Environment Variable: %s       Variable Value: %s \n", inEnvVars[i]->getName().c_str(), inEnvVars[i]->getValue().c_str());
		}
	}else if(myCommand == "setdir"){
		//set shell's concept of current directory to directory_name (See getwd(3) and chdir(2))
		myDirectoryName = myArguments[0];
		//note: uncomment #include <unistd.h>
		//chdir changes the directory to the given path. 
		//But chdir takes  a const char* and myDirectoryName is a string. 
		if (chdir(myDirectoryName.c_str() != 0) {	//Correct behavior returns 0
			//something went wrong
			printf("Path Error, did not change directory.");
		}

		//Free the memory of the environment array
		for (int i = 0; i < inEnvVars.size(); i++) {
			delete[] envArray[i];
		}

		delete[] envArray;

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

			char ** envArray = envVarConvert(inEnvVars.size(), inEnvVars);
			char ** argArray = argConvert(myArguments.size());
			execve(myCommand.c_str(), argArray, envArray);			//Do we need to pass the environment variable to this?

			//should never reach the next line			Alex: why not?
			printf("Error in the command.");

			//Free memory if it does reach this line?
			//Free the memory of the environment array
			for (int i = 0; i < inEnvVars.size(); i++) {
				delete[] envArray[i];
			}

			delete[] envArray;

			//Free the memory of the arguments array
			for (int i = 0; i < myArguments.size(); i++) {
				delete[] argArray[i];
			}

			delete[] argArray;
		}
		else{
			if (waitpid(kidpid, 0, 0) < 0){
				printf("Cannot wait for child.");
			}
		}
	}

	return true;
}

char ** Parser::envVarConvert(int rows, vector<EnvVar*> & inEnvVars) {
	char ** returnArray;
	returnArray = new char *[rows];	//initialize number of variables
	for (int i = 0; i < rows; i++) {
		returnArray[i] = new char[256];			//Each var has room for 256 chars
		char * copy = new char[256];
		strcpy(copy, inEnvVars[i]->getValue().c_str());	//Workaround to convert a const char * to a char *
		returnArray[i] = copy;

		delete[] copy;
	}

	return returnArray;
}

char ** Parser::argConvert(int rows) {
	char ** returnArray;
	returnArray = new char *[rows];	//initialize number of variables
	for (int i = 0; i < rows; i++) {
		returnArray[i] = new char[256];			//Each var has room for 256 chars
		char * copy = new char[256];
		strcpy(copy, myArguments[i].c_str());	//Workaround to convert a const char * to a char *
		returnArray[i] = copy;

		delete[] copy;
	}

	return returnArray;
}

//Searches a vector of EnvVar and returns the index item is found at, or -1 if not found. Will be relatively small so iterative search is fine.
int Parser::vectorSearch(string name, vector<EnvVar*> & vector) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i]->getName() == name) {
			return i;
		}
	}

	return -1;
}