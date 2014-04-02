#ifndef TOKEN_H
#define TOKEN_H
#include <string>
using namespace std;

class Token {
public:
	Token(string inType, string inValue);

	string getType();
	string getValue();

private:
	string type;
	string value;
};

#endif