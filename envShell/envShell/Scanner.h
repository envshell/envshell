#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <vector>
#include "Token.h"

class Scanner {
public:
	Scanner();
	vector<Token*> scan(string inCommand);

private:
	vector<Token*> tokens;
};

#endif