#ifndef ENVVAR_H
#define ENVVAR_H

#include <string>

class EnvVar{
public:
	EnvVar(std::string inName, std::string inValue);

	std::string getName();
	std::string getValue();

private:
	std::string name;
	std::string value;
};

#endif