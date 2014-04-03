#include "EnvVar.h"
using namespace std;

EnvVar::EnvVar(string inName, string inValue) {
	name = inName;
	value = inValue;
}

string EnvVar::getName() {
	return name;
}

string EnvVar::getValue() {
	return value;
}