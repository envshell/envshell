#include "Scanner.h"
#include <vector>
#include <string>
using namespace std;

Scanner::Scanner() {}

vector<Token> Scanner::scan(string inCommand) {
	char * command = new char[256]();
	command = inCommand.c_str();
	char * pch;
	printf("Splitting string \"%s\" into tokens:\n", str);
	pch = strtok(str, " ,.-");
	while (pch != NULL)
	{
		printf("%s\n", pch);
		pch = strtok(NULL, " ,.-");
	}
}