#include<iostream>

using namespace std;

int main()
{
	int x = 10;
	x += 10;//same as x = x+10;
	cout <<x<<endl;
	x -= 5;// x = x -5;
	cout <<x<<endl;
	x *= 10;
	cout <<x<<endl;
	x/=5;
	cout <<x<<endl;
	cout <<x++<<endl;//post increment operator....first processes x according to cout then increments it
	cout <<x<<endl;
	cout <<++x<<endl;//pre increment operator.....first increases x then processes it according to cout
}
