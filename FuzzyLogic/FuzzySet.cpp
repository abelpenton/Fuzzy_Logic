
#include <string>
#include <vector>
#include <iostream>
#include "Membership.h"
using namespace std;

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
			cout << "No adequate MF" << endl;
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