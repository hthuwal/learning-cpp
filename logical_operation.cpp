#include <iostream>

using namespace std;

int main()
{
	int age = 23;
	int money = 650;
	/*if(age>21)
	{
		if(money>500)
		{
			cout<<"You are allowed in";
		}
	}*/
	if (age > 21 && money > 500) // && logical and--both must be true for entire to be true
	{
		cout << "you are allowed in !!\n";
	}
	if (age > 21 || money > 500) // || logical or ---any of them must be true
	{
		cout << "you are allowed in too!! :P" << endl;
	}
	return 0;
}
