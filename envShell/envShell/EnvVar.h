#include <string>

class EnvVar{
public:
	EnvVar(string inName, string inValue);

	std::string getName();
	std::string getValue();

private:
	std::string name;
	std::string value;
};