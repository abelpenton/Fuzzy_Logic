// FuzzyLogic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <stdio.h>
using namespace std;

double And(double a, double b) {
	return fmin(a, b);
}

double Or(double a, double b)
{
	return fmax(a, b);
}

double And(std::vector<double> &values) {
	double and = values.front();

	for (int i = 0; i < values.size(); i++)
	{
		and = And(and, values[i]);
	}

	return and;
}

double Or(std::vector<double> &values)
{
	double or = values.front();

	for (int i = 0; i < values.size(); i++)
	{
		or = Or(or , values[i]);
	}

	return or ;
}

struct Rules
{
	std::vector<std::string> rules;

	void addRule(std::string r)
	{
		rules.push_back(r);
	}

	std::multimap<std::string, double> inferMamdani(std::map<std::string, double> inputMembershipValues)
	{
		std::multimap<std::string, double> outputMembershipValues;

		for (int i = 0, j = 3; i < rules.size(); i++)
		{
			j = 3;
			std::string rule = rules[i];
			std::string fuzzySetName = "";


			bool andFlag = false, orFlag = false;
			double accum = 0;

			while (fuzzySetName != "THEN" && fuzzySetName != "then")
			{
				fuzzySetName = "";

				while (rule[j] != ' ')
				{
					fuzzySetName += rule[j++];
				}


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


			outputMembershipValues.insert(std::pair<std::string, double>(fuzzySetName, accum));

		}

		return outputMembershipValues;
	}

	std::map<std::string, double> DesFuzzyCentroide(std::multimap<std::string, double> out, int min, int max)
	{
		double num = 0;
		double den = 0;
		std::map<std::string, double> output;
		for (size_t i = min; i < max; i+=10)
		{
			for (std::multimap<std::string, double>::iterator itr = out.begin(); itr != out.end(); ++itr)
			{
				num += (*itr).second * i;
				den += (*itr).second;

			}
		}

		for (std::multimap<std::string, double>::iterator itr = out.begin(); itr != out.end(); ++itr)
		{
			if ((*itr).second != 0) {
				output[(*itr).first] = num / den;
			}

		}
		return output;
	}

	std::map<std::string, double> DesFuzzyMax(std::multimap<std::string, double> out)
	{
		std::map<std::string, double> output;

		std::string currentSet = "";
		double maximum = 0;


		for (std::multimap<std::string, double>::iterator itr = out.begin(); itr != out.end(); ++itr)
		{
			if ((*itr).first != currentSet)
			{
				maximum = 0;
				currentSet = (*itr).first;
			}

			maximum = std::fmax(maximum, (*itr).second);
			output[currentSet] = maximum;
		}

		output[currentSet] = maximum*100;

		return output;

	}

	std::map<std::string, double> DesFuzzyMaxMean(std::multimap<std::string, double> out)
	{
		std::map<std::string, double> output;

		std::string currentSet = "";
		double maximum = 0;
		double mean = 0;
		int c = 0;
		for (std::multimap<std::string, double>::iterator itr = out.begin(); itr != out.end(); ++itr)
		{
			if ((*itr).first != currentSet)
			{
				currentSet = (*itr).first;
			}
			maximum = std::fmax(maximum, (*itr).second);
		}

		for (std::multimap<std::string, double>::iterator itr = out.begin(); itr != out.end(); ++itr)
		{
			if (maximum == (*itr).second) {
				mean += maximum;
				c++;
			}
		}
		output[currentSet] = (mean/c)*100;


		return output;

	}
};




double triangmf(double a, double b, double c, double x)
{
	double o1 = x - a;
	double o2 = b - a;
	double o3 = c - x;
	double o4 = c - b;
	return fmax(fmin(o1 / o2, o3 / o4), 0);
}

double trapmf(double a, double b, double c, double d, double x)
{
	double o1 = x - a;
	double o2 = b - a;
	double o3 = d - x;
	double o4 = d - c;
	return fmax(fmin(o1 / o2, fmin(1, o3 / o4)), 0);
}


double gaussianmf(double center, double width, double x)
{
	return exp(-pow((x - center) / (sqrt(2 * width)), 2));
}
enum MFType { TRIANG, TRAP, GAUSS };

struct FuzzySet
{
	string name;
	MFType type;
	vector<double> params;

	FuzzySet(string n) : name(n) {}

	double eval(double x)
	{
		double res = 0;

		switch (type)
		{
		case TRIANG:
			if (params.size() == 3)				res = triangmf(params[0], params[1], params[2], x);
			break;

		case TRAP:
			if (params.size() == 4)				res = trapmf(params[0], params[1], params[2], params[3], x);
			break;


		case GAUSS:
			if (params.size() == 2)				res = gaussianmf(params[0], params[1], x);
			break;

		default:
			break;
		}

		return res;
	}

	void setMF(MFType t, double *args, int size)
	{
		type = t;

		for (int i = 0; i < size; i++)
		{
			params.push_back(args[i]);
		}
	}
};

int main()
{
	double temp = 10;
	double humed = 5;

	FuzzySet humedad_seca("Humedad_Seca");
	FuzzySet humedad_humeda("Humedad_Humeda");
	FuzzySet humedad_mojado("Humedad_Mojado");

	double args1[] = { 0, 1, 8, 12 };
	humedad_seca.setMF(TRAP, args1, 4);
	double args2[] = { 8, 12, 30, 35 };
	humedad_humeda.setMF(TRAP, args2, 4);
	double args3[] = { 20, 40, 70, 100 };
	humedad_mojado.setMF(TRAP, args3, 4);



	FuzzySet temp_congelada("Temperatura_Congelada");
	FuzzySet temp_fria("Temperatura_Fria");
	FuzzySet temp_normal("Temperatura_Normal");
	FuzzySet temp_tibia("Temperatura_Tibia");
	FuzzySet temp_caliente("Temperatura_Caliente");

	double args4[] = { -100, -60, 0, 13 };
	temp_congelada.setMF(TRAP, args4, 4);
	double args5[] = { 10, 15, 20 };
	temp_fria.setMF(TRIANG, args5, 3);
	double args6[] = { 15,21,23 };
	temp_normal.setMF(TRIANG, args6, 3);
	double args7[] = { 20,25,35 };
	temp_tibia.setMF(TRIANG, args7, 3);
	double args8[] = { 24,44,70,100 };
	temp_caliente.setMF(TRAP, args8, 4);


	FuzzySet riego_corto("Riego_Corto");
	FuzzySet riego_medio("Riego_Medio");
	FuzzySet riego_prolongado("Riego_Prolongado");

	double args9[] = { 0,1,10,20 };
	riego_corto.setMF(TRAP, args9, 4);
	double args10[] = {10,25,38,55 };
	riego_medio.setMF(TRAP, args10, 4);
	double args11[] = { 45,60,90,110 };
	riego_prolongado.setMF(TRAP, args11, 4);


	map<string, double> fuzzy;

	fuzzy[humedad_seca.name] = humedad_seca.eval(humed);
	fuzzy[humedad_mojado.name] = humedad_mojado.eval(humed);
	fuzzy[humedad_humeda.name] = humedad_humeda.eval(humed);



	fuzzy[temp_caliente.name] = temp_caliente.eval(temp);
	fuzzy[temp_congelada.name] = temp_congelada.eval(temp);
	fuzzy[temp_fria.name] = temp_fria.eval(temp);
	fuzzy[temp_normal.name] = temp_normal.eval(temp);
	fuzzy[temp_tibia.name] = temp_tibia.eval(temp);

	Rules rules;

	rules.addRule("IF Humedad_Seca AND Temperatura_Congelada THEN Riego_Prolongado");
	rules.addRule("IF Humedad_Seca AND Temperatura_Fria THEN Riego_Prolongado");
	rules.addRule("IF Humedad_Seca AND Temperatura_Normal THEN Riego_Prolongado");
	rules.addRule("IF Humedad_Seca AND Temperatura_Tibia THEN Riego_Prolongado");
	rules.addRule("IF Humedad_Seca AND Temperatura_Caliente THEN Riego_Prolongado");



	rules.addRule("IF Humedad_Mojado AND Temperatura_Congelada THEN Riego_Corto");
	rules.addRule("IF Humedad_Mojado AND Temperatura_Fria THEN Riego_Corto");
	rules.addRule("IF Humedad_Mojado AND Temperatura_Normal THEN Riego_Corto");
	rules.addRule("IF Humedad_Mojado AND Temperatura_Tibia THEN Riego_Corto");
	rules.addRule("IF Humedad_Mojado AND Temperatura_Caliente THEN Riego_Corto");


	rules.addRule("IF Humedad_Humeda AND Temperatura_Congelada THEN Riego_Medio");
	rules.addRule("IF Humedad_Humeda AND Temperatura_Fria THEN Riego_Medio");
	rules.addRule("IF Humedad_Humeda AND Temperatura_Normal THEN Riego_Medio");
	rules.addRule("IF Humedad_Humeda AND Temperatura_Tibia THEN Riego_Medio");
	rules.addRule("IF Humedad_Humeda AND Temperatura_Caliente THEN Riego_Medio");


	multimap<string, double> mamdani = rules.inferMamdani(fuzzy);

	map<string, double> desfuzzy = rules.DesFuzzyMaxMean(mamdani);

	for (map<string, double>::iterator itr = desfuzzy.begin(); itr != desfuzzy.end(); ++itr)
	{
		string label = (*itr).first;
		printf(label.data());
		cout << ": ";
		cout << (*itr).second << endl;
	}
	return 0;
}

