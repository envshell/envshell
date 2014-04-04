#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "Scanner.h"
#include "Parser.h"
using namespace std;

int main() {

	string prompt = "envsh > ";
	bool loop = true;

	
	while(loop){
		printf("%s", prompt.c_str());
		//fgets(char* str, int n, FILE* stream)
		//Fgets(command, 256, c.in)
		string command = "/usr/bin/cat /etc/issue /etc/hosts /etc/debian_version > /tmp/output";
		Parser* p = new Parser(command);
		loop = p->parse(prompt);
	}

	cin.ignore(200, '\n');
	cin.ignore(200, '\n');

	return 0;
}