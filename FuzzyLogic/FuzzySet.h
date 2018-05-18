#include <string>
#include <vector>

using namespace std;

enum MFType { TRIANG, TRAP, GAUSS };

struct FuzzySet
{
	string name;
	MFType type;
	vector<double> params;

	FuzzySet(string n) : name(n) {}

	double eval(double x);

	void setMF(MFType t, double *args, int size);
};