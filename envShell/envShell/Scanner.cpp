#include "Scanner.h"
#include "Token.h"
#include <vector>
#include <string>
using namespace std;

Scanner::Scanner() {}

vector<Token*> Scanner::scan(string inCommand) {
	bool invalid = false;
	string type;
	string value;

	//Check the validity of the first character of the command
	if (inCommand[0] != '%' && !isalpha(inCommand[0])){
		invalid = false;	//first char must be '%' or an alphabetic letter
		//throw exception
	}

	//Handle if the first value is a %
	if (inCommand[0] == '%') {
		//Add to tokens
		type = "metachar";
		value = '%';
		tokens.push_back(new Token(type, value));

		type = "string";
		value = inCommand.substr(1, string::npos);
		tokens.push_back(new Token(type, value));

		//return the tokens vector
		return tokens;
	}

	//read the first word. We know it is a word because otherwise we would've returned the comment already
	int spacei = inCommand.find(" ");
	if (spacei == string::npos) { //if no spaces it is invalid
		invalid = true;
		//throw exception
	}

	string firstValue = inCommand.substr(0, spacei);
	string firstType = "word";
	tokens.push_back(new Token(firstType, firstValue));
	spacei++;

	//Iterate through rest of string looking for delimiters
	for (int i = spacei; i < inCommand.length(); i++) {
		char c = inCommand[i];

		//handle if the input is ^d


		//Take action depending on the character present
		if (c == '<' || c == '>') {
			//metachar handling
			type = "metachar";
			value = "" + c;
		}
		else if (c == '"') {
			//string handling
			value;
			type = "string";

			bool found = false;		//Found second set of quotes
			int endi = i;
			while (!found) {
				if (endi >= inCommand.length()) {
					//Second quotes not found, incorrect string
					//throw exception
				}
				if (inCommand[endi] == '"') {
					//found the closing quote
					found = true;
					value = inCommand.substr(i, endi);
				}

				endi++;
			}

			//Now add to our vector of Tokens
			tokens.push_back(new Token(type, value));

			//Now advance i to the end of the string
			i = endi;
		}
		else if (isalpha(c)) {
			//Handle if it is a word
			type = "word";
			value = inCommand.substr(i, inCommand.find(" ", i+1));
		}
	}

	//Now add an end of line token
	type = "eol";
	value = "eol";
	tokens.push_back(new Token(type, value));

	//Finally return our tokens
	return tokens;
}