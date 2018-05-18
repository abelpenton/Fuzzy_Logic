
#include <vector>
#include <cmath>

double And(double a, double b) {
	return fmin(a,b);
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

	return or;
}

