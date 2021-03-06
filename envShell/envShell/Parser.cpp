


#include "Parser.h"
#include "Scanner.h"
#include "EnvVar.h"
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include <fcntl.h>
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
	/*cout << "myCommand = " << myCommand << endl;
	for(int i = 0; i< myTokens.size(); i++){
		cout << myTokens[i]->getValue() << endl;
	}*/
	//store arguments until hit "<" or EOL
	int i = 1;
	while (myTokens[i]->getValue() != "<" && myTokens[i]->getValue() != ">" && (myTokens[i]->getValue() != "EOL" && myTokens[i]->getType() != "end-of-line")){
		myArguments.push_back(myTokens[i]->getValue());
		i++;
	}
	//check to see if there is an input file
	if(myTokens[i]->getValue() == "<"){
		i++;
		myInfile = myTokens[i]->getValue();
		inFile = true;
		i++;
	}else{inFile = false;}
	//check to see if there is an output file
	if(myTokens[i]->getValue() == ">"){
		i++;
		myOutfile = myTokens[i]->getValue();
		
		//Debug print statement
		//cout << "Output file is: " << myOutfile << endl;
		
		outFile = true;
	}else{outFile = false;}

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
		cout << "Prompt set to: " << prompt << endl;
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
		//EnvVar* element = new EnvVar(myArguments[0], myArguments[1]);

		//find element in the environmentVariable list and delete
		int index = vectorSearch(myArguments[0], inEnvVars);
		if (index >= 0) {
			inEnvVars.erase(inEnvVars.begin() + index);
			cout << "Removed " << myArguments[0] << " from the list." << endl;
			return true;
		}
		else {
			cout << "Variable not found." << endl;
		}
	}else if(myCommand == "listenv"){
		//prints the list of environment variables and their values
		if(inEnvVars.size() == 0){
			cout << "There are no environment varaibles set." << endl;
		}else{
			for(int i = 0; i < inEnvVars.size(); i++){
				printf("Environment Variable: %s       Variable Value: %s \n", inEnvVars[i]->getName().c_str(), inEnvVars[i]->getValue().c_str());
			}
		}
	}else if(myCommand == "setdir"){
		//set shell's concept of current directory to directory_name (See getwd(3) and chdir(2))
		myDirectoryName = myArguments[0];
		//note: uncomment #include <unistd.h>
		//chdir changes the directory to the given path. 
		//But chdir takes  a const char* and myDirectoryName is a string. 
		if (chdir(myDirectoryName.c_str()) != 0) {	//Correct behavior returns 0
			//something went wrong
			printf("Path Error, did not change directory.\n");
		}
		else {
			cout << "Directory has been changed to " << myDirectoryName << endl;
		}
	}else if(myCommand == "bye"){
		//exit the shell program
		return false;
	}else if(myCommand[0] == 0x04){
		//ctrl+D entered, exit the shell program
		//Doesn't work but our program handles end of file input anyway so the Ctrl+D handling is
		//unnecessary
		return false;
	}else{
		//the command is a user-program command, need to use fork() and wait() until the child finishes
		//execv(const char *path, char *const argv[]); 

		int retval;
		int child_status;
		pid_t pid = fork();
		char ** envArray = envVarConvert(inEnvVars.size(), inEnvVars);
		char ** argArray = argConvert(myCommand, myArguments.size());

		if(pid == 0){
			//This is the child

			//int i = execle(argArray[0], argArray[1], NULL, envArray);	//This works with different results
			int i;
			int fdw;
			int fdr;
			if(inFile){
				const char * infile = myInfile.c_str();
                                fdr = open(infile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // remember to close
                                if(fdr<0){
                                    //error in opening
									cout << "Error in opening the file " << myInfile << endl;
                                }
				//sets the stdin to be directed from the input file
				dup2(fdr, 0);
			}
			
			if(outFile){
				const char * outfile = myOutfile.c_str();
				fdw = open(outfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // remember to close
				if(fdw<0){
        	    //error in opening
				cout << "Error opening the file " << myOutfile << endl;
	            }
				//sets the stdout to be directed into the output file
				dup2(fdw,1);
			}
			
			i = execvpe(argArray[0], argArray, envArray);
			if(outFile){
				int returnvalue;
				if((returnvalue = close(fdw)) < 0){
					//error closing
					cout << "Error closing the outputfile" << endl;
				}
			}
			if(inFile){
                       int returnvalue;
                       if((returnvalue = close(fdr)) < 0){
						   //error closing
						   cout << "Error closing the input file" << endl;
                       }
            }

			if (i < 0) {
				cout << "The command " << myCommand << " could not be found." << endl;

				exit(0);
			}

			//Does not reach this line
		}
		else {
			retval = wait(&child_status);		//Block until child terminates
		}
	}

	return true;
}

//Does this also need to be null terminated????
char ** Parser::envVarConvert(int rows, vector<EnvVar*> & inEnvVars) {
	char ** returnArray;
	returnArray = new char *[rows];	//initialize number of variables
	for (int i = 0; i < rows; i++) {
		returnArray[i] = new char[256];			//Each var has room for 256 chars
		
		//Build string of the form name=value
		string var = "" + inEnvVars[i]->getName() + "=" + inEnvVars[i]->getValue();
		char * copy = new char[256];
		strcpy(copy, var.c_str());	//Workaround to convert a const char * to a char *
		returnArray[i] = copy;

		delete[] copy;
	}

	return returnArray;
}

//Add the path of the file as the first argument in the array and NULL terminate this array
char ** Parser::argConvert(string myCommand, int rows) {
	char ** returnArray;
	returnArray = new char *[rows + 2];	//initialize number of variables

	//Make the command the first argument
	returnArray[0] = new char[256];			//Each var has room for 256 chars
	char * workaround = new char[256];
	strcpy(workaround, myCommand.c_str());	//Workaround to convert a const char * to a char *
	returnArray[0] = workaround;
	delete[] workaround;

	for (int i = 1; i <= rows; i++) {
		returnArray[i] = new char[256];			//Each var has room for 256 chars
		char * copy = new char[256];
		strcpy(copy, myArguments[i - 1].c_str());	//Workaround to convert a const char * to a char *
		returnArray[i] = copy;

		delete[] copy;
	}

	returnArray[rows + 1] = NULL;

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
