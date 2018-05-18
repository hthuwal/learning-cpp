#include<iostream>

using namespace std;
void print(int array[], int y)
{
	for(int x=0;x<y;x++)
	{
		cout <<array[x]<<endl;
	}
}
int main()
{
	int hc[3]={20,54,675};
	int jp[6]={54,24,7,8,9,99};
	print(hc,3);
	print(jp,6);
}
