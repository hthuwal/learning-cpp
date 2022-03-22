#include <iostream>

using namespace std;

int main()
{
	int i = 1, a, sum = 0;
	while (i <= 5)
	{
		cin >> a;
		sum = sum + a; // new value of sum = old value of sum+ a
		i++;
	}
	cout << sum;
	return 0;
}
