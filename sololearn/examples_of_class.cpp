#include<iostream>

using namespace std;

class TestClass //generally named in capital letters
{
	public://anything after this can be used outside class and public is called an access specifier	
		void Test()
		{
			cout << "Testing 1 2 3"<<endl;
		}
};	
int main()
{
	TestClass obj;	
	obj.Test();
	return 0;
}
