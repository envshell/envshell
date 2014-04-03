#include <string>
#include <vector>
#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;

int main() {
	string prompt = "envsh > ";

	//Test the scanner implementation
	string command = "/usr/bin/cat /etc/issue /etc/hosts /etc/debian_version > /tmp/output";
	Parser* p = new Parser(command);
	p->parse(prompt);

	

	cin.ignore(200, '\n');
	cin.ignore(200, '\n');
	
}