#include <iostream>

using namespace std;

int main()
{
	int x = 4 + 6;
	cout << x << endl;
	x = 8 - 4;
	cout << x << endl;
	x = 8 * 4;
	cout << x << endl;
	x = 81 / 3;
	cout << x << endl;
	x = 81 / 2;
	cout << x << endl;
	x = 81 % 2;
	cout << x << endl;
	x = 6 * 4 + 8 * 4 + 9 / 20;
	//  order of precedence paranthesis>multiplication=division>addition=subtraction
	cout << x << endl;
	return 0;
}
