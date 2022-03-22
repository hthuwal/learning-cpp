#include <iostream>

using namespace std;

int main()
{
	int hc[9];
	cout << "Element - Value" << endl;
	for (int i = 0; i < 9; i++)
	{
		hc[i] = i * i;
		cout << i << "	  " << hc[i] << endl;
	}
}
