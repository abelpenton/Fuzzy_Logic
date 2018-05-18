
#include <cmath>

double triangmf(double left, double center, double right, double x)
{
	double res = 0;

	if (x <= left)							res = 0;
	else if (x >= right)						res = 0;
	else if (x > left && x <= center)		res = (x - left) / (center - left);
	else if (x < right && x > center)		res = 1 - abs((right - x) / (center - right));


	return res;
}

double trapmf(double lowLeft, double upLeft, double upRight, double lowRight, double x)
{
	double res = 0;

	if (x <= lowLeft)						res = 0;
	else if (x >= lowRight)					res = 0;
	else if (x > lowLeft && x <= upLeft)		res = (x - lowLeft) / (upLeft - lowLeft);
	else if (x > upLeft && x <= upRight)		res = 1;
	else if (x > upRight && x < lowRight)	res = 1 - abs((upRight - x) / (lowRight - upRight));

	return res;
}


double gaussianmf(double center, double width, double x)
{
	return exp(-pow((x - center) / (sqrt(2 * width)), 2));
}