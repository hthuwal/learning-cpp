#include<iostream>
/*if one is using fuction prototyping provide default values only during prototyping not when defining function 
in case one doesnt does prototyping default values should be put while defining the function*/
using namespace std;
int volume(int l=1,int w=1,int h=1)//values given to variables are default argument in case user gives no or few arguments
{
	return l*w*h;
}

int main()
{
	cout<< volume()<<endl;
	cout<< volume(5)<<endl;
	cout<< volume(5,6)<<endl;
	cout<< volume(5,6,5)<<endl;
}

