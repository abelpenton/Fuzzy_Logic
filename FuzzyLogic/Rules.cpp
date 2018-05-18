
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

	void addRule(std::string r)
	{
		rules.push_back(r);
	}

	/* Inferencia Mamdani. Como en Matlab, una regla sólo puede contener ANDs, o sólo puede contener ORs (por el momento) */
	std::map<std::string, double> inferMamdani(std::map<std::string, double> inputMembershipValues)
	{
		std::multimap<std::string, double> outputMembershipValues;


		//j = 3 para ignorar el "IF "
		for (int i = 0, j = 3; i < rules.size(); i++)
		{
			j = 3;
			std::string rule = rules[i];
			std::string fuzzySetName = "";

			std::cout << "IF" << std::endl;

			bool andFlag = false, orFlag = false;
			double accum = 0;

			while (fuzzySetName != "THEN" && fuzzySetName != "then")
			{
				fuzzySetName = "";

				while (rule[j] != ' ')
				{
					fuzzySetName += rule[j++];
				}

				std::cout << "fuzzySetName - " << fuzzySetName << std::endl;

				if (fuzzySetName == "AND" || fuzzySetName == "and")			andFlag = true;
				else if (fuzzySetName == "OR" || fuzzySetName == "or")		orFlag = true;
				else
				{
					if (inputMembershipValues.find(fuzzySetName) != inputMembershipValues.end())
					{
						double currentValue = inputMembershipValues[fuzzySetName];

						if (andFlag)
						{
							andFlag = false;
							accum = And(currentValue, accum);
						}

						else if (orFlag)
						{
							orFlag = false;
							accum = Or(currentValue, accum);
						}

						//Sólo se va a entrar una vez a este else (para el primer valor de la regla, antes de cualquier AND u OR)
						else
						{
							accum = currentValue;
						}
					}
				}

				j++;
			}

			fuzzySetName = "";

			while (j < rule.length())
			{
				fuzzySetName += rule[j];
				j++;
			}

			std::cout << "fuzzySetName - " << fuzzySetName << std::endl;
			
			outputMembershipValues.insert(std::pair<std::string, double>(fuzzySetName, accum));

		}

		std::map<std::string, double> output;

		std::string currentSet = "";
		double maximum = 0;


		for (std::multimap<std::string, double>::iterator itr = outputMembershipValues.begin(); itr != outputMembershipValues.end(); ++itr)
		{
			if ((*itr).first != currentSet)
			{
				maximum = 0;
				currentSet = (*itr).first;
			}

			maximum = std::fmax(maximum, (*itr).second);
			output[currentSet] = maximum;
		}

		output[currentSet] = maximum;

		return output;
	}

};
