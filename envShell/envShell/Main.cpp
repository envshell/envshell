#include <string>
#include <vector>
#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;

int main() {
	//Test the scanner implementation
	string command = "/usr/bin/cat /etc/issue /etc/hosts /etc/debian_version > /tmp/output";
	Parser* p = new Parser(command);
	p->parse();

	

	cin.ignore(200, '\n');
	cin.ignore(200, '\n');
	
}