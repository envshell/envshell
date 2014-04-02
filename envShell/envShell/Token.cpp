#include "Token.h"

Token::Token(string inType, string inValue) {
	type = inType;
	value = inValue;
}

string Token::getType() {
	return type;
}

string Token::getValue() {
	return value;
}

