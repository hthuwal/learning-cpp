#include <iostream>

using namespace std;

int main()
{
	int age;
	cin >> age;
	switch (age)
	{
	case 16:
		cout << "Hey you can drive a car now!!\n";
		break;
	case 18:
		cout << "Go buy some lottery tickets!!\n";
		break;
	case 21:
		cout << "Buy me some beer!!\n";
		break; // break --> end the switch
	default:
		cout << "Do whatever the fuck you want :P\n";
	}
	return 0;
}
