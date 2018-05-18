#include <iostream>
// based on idea that a function can call itself//
// the function must have a stopping condition or a base case otherwise it will never end or would run infintely//
using namespace std;

int fact(int x)
{
	if (x == 1)
		return 1;
	else
		return x * fact(x - 1);
}

int main()
{
	cout << fact(5);
	return 0;
}
