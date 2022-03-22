#include <iostream>

using namespace std;

int main()
{
	int *p = new int;
	*p = 5;
	cout << *p << endl;
	delete p; // deletes the memory allocated to 5
	// at this moment p points to nothing not a valid memory hence called a dangling pointer
	p = new int(10);
	cout << *p << endl;
}
