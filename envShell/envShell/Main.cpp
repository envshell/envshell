#include <string>
#include <vector>
#include <iostream>
#include "Scanner.h"
using namespace std;

int main() {
	//Test the scanner implementation
	string command = "setdir ./bin";
	Scanner * scanner = new Scanner();

	vector<Token*> tokens;
	try {
		tokens = scanner->scan(command);
	}
	catch(int err) {
		cout << "\nException thrown. Error Number: " << err << endl;
		//printf("\nException thrown. Error Number: %n \n", err);
	}
	for (int i = 0; i < tokens.size(); i++) {
		cout << "Type: " << tokens[i]->getType() << "     Token: " << tokens[i]->getValue() << endl;
		//printf("\nType: %t       Token: %n \n", tokens[i]->getType(), tokens[i]->getValue());
	}

	cin.ignore(200, '\n');
	cin.ignore(200, '\n');
	
}