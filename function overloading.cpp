#include<iostream>

using namespace std;

//void printNumber(int x) - > passing integer only not gud :( //
//void printNumber(float x) -> passing float only not gud :( //
//function overloading in c++ allows us to use same name for two functinos :)//
void printNumber(int x)//number 1
{
	cout<< "i am printing an integer "<<x<<endl;
}
void printNumber(float x)//number 2
{
	cout<< "i am printing a float "<<x<<endl;
}
int main()
{
	int a = 54;
	float b = 32.4896;
	printNumber(a);//would call function number 1 as argument is an integer
	printNumber(b);//would call function number 2 as argument is a float
	return 0;
}

