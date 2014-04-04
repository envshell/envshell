#include "Scanner.h"
#include "Token.h"
#include <vector>
#include <string>
using namespace std;

Scanner::Scanner() {}

Scanner::~Scanner() {
	for (int i = 0; i < tokens.size(); i++) {
		delete tokens[i];
	}
}

vector<Token*> Scanner::scan(string inCommand) {
	string type;
	string value;

	//handle if there is an input of "^d" on the line
	if (inCommand.find("^D") != string::npos) {
		return tokens;
	}

	//Check the validity of the first character of the command
	if (inCommand[0] != '%' && !isalpha(inCommand[0]) && inCommand[0] != '/') {
		throw 1;		//Invalid input string exception
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

		//Now add an end of line token
		type = "end-of-line";
		value = "EOL";
		tokens.push_back(new Token(type, value));

		//return the tokens vector
		return tokens;
	}

	//read the first word. We know it is a word because otherwise we would've returned the comment already
	//int spacei = inCommand.find(" ");
	bool found = false;
	int spacei = -1;
	int j = 0;
	while (!found && j < inCommand.length()) {
		if (isspace(inCommand[j]) || inCommand[j] == '<' || inCommand[j] == '>') {
			found = true;

			//Advance until the next non-whitespace char
			while (isspace(inCommand[j])) {
				j++;
			}

			spacei = j;
		}
		
		j++;
	}

	if (j == inCommand.length()) {
		//Reached end, this is one large word
		spacei = inCommand.length();
	}
	else if (!found) { //if no spaces it is invalid
		throw 1;		//Invalid input string exception
	}

	string firstValue = inCommand.substr(0, spacei - 1);
	string firstType = "word";
	tokens.push_back(new Token(firstType, firstValue));

	//Iterate through rest of string looking for delimiters
	for (int i = spacei; i < inCommand.length(); i++) {
		char c = inCommand[i];

		//Take action depending on the character present
		if (c == '<' || c == '>') {
			//metachar handling
			type = "metachar";
			value = "" + c;

			tokens.push_back(new Token(type, value));
		}
		else if (c == '"') {
			//string handling
			value;
			type = "string";

			//Find the enxty set of quotations
			int quotei = inCommand.find('"', i + 1);
			value = inCommand.substr(i, quotei - i);

			//Now add to our vector of Tokens
			tokens.push_back(new Token(type, value));

			//Now advance i to the end of the string
			i = quotei;
		}
		else if (!isspace(c)) {
			//Handle if it is a word
			type = "word";

			//find the next delimiting character
			int j = i + 1;
			int originalPos = i;
			int delimi = -1;
			while ((!isspace(inCommand[j]) && inCommand[j] != '<' && inCommand[j] && '>') && j < inCommand.length()) {
				char charher = inCommand[j];
				bool space = isspace(inCommand[j]);
				j++;
			}

			//if reached end of string
			if (j >= inCommand.length()) {
				value = inCommand.substr(originalPos, inCommand.length() - originalPos);

				tokens.push_back(new Token(type, value));
				i = inCommand.length();	//advance index
			}
			else if (isspace(inCommand[j])) {
				delimi = j;

				value = inCommand.substr(originalPos, delimi - originalPos);
				tokens.push_back(new Token(type, value));
				i = delimi;
			}
			else if (inCommand[j] == '<' || inCommand[j] == '>') {
				delimi = j;

				value = inCommand.substr(originalPos, delimi - originalPos);
				tokens.push_back(new Token(type, value));
				i = delimi - 1;
			}
			else {
				//Something has gone wrong
				throw 99;
			}
		}
	}

	//Now add an end of line token
	type = "end-of-line";
	value = "EOL";
	tokens.push_back(new Token(type, value));

	//Finally return our tokens
	return tokens;
}