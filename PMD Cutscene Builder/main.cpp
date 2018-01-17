#include "ScriptUI.h"
#include <iostream>

int gcd(float a, float b)
{
	// Everything divides 0 
	if (a == 0.f || b == 0.f)
		return 0;

	// base case
	if (a == b)
		return int(a);

	// a is greater
	if (a > b)
		return gcd(a - b, b);
	return gcd(a, b - a);
}

int main()
{



	ScriptUI sui;
	sui.run();

	return 0;
}
