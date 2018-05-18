#include <string>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Operator.h"
#include <cmath>

struct Rules
{
	std::vector<std::string> rules;
	void addRule(std::string r);
	std::map<std::string, double> inferMamdani(std::map<std::string, double> inputMembershipValues);

};
