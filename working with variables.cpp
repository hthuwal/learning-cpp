/*sum of two numbers*/
#include<iostream>

using namespace std;

int main()
{
	int a;//declaring the variable
	int b;
	int sum;
	cout <<"Enter a number!\n";
	cin >> a;//cin is a  input stream object// >> stream extraction operator
	cout <<"Enter another number!\n";
	cin >> b;
	sum = a+b;
	cout <<"The sum of those numbers is "<<sum<<endl;
	return 0;
}
