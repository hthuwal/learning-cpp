#include <iostream>

using namespace std;

void passbyvalue(int x) // makes changes to the copy of argument not the acutal variable// x is mere copy of var1 has no access to var1 itself
{
	x = 99;
}
void passbyreference(int *x) // makes changes to the original variable as its address is copied and the same address is being dereferenced
{
	*x = 99;
}
int main()
{
	int var1 = 13;
	int var2 = 13;
	passbyvalue(var1);
	passbyreference(&var2);
	cout << "var1 is now " << var1 << endl;
	cout << "var2 is now " << var2 << endl;
	return 0;
}
