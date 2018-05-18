#include<iostream>

using namespace std;

void passbyvalue(int x)//makes changes to the copy of argument not the acutal variable// x is mere copy of hc has no access to hc itself
{
	x=99;
	
}
void passbyreference(int *x)//makes changes to the original variable as its address is copied and the same address is being dereferenced
{
	*x=99;
}
int main()
{
	int hc = 13;
	int jp = 13;
	passbyvalue(hc);
	passbyreference(&jp);
	cout<<"hc is now "<<hc<<endl;
	cout<<"jp is now "<<jp<<endl;
	return 0;
}
