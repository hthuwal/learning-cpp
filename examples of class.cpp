#include<iostream>

using namespace std;

class harish //generally named in capital letters
{
	public://anything after this can be used outside class and public is called an access specifier	
		void hc()
		{
			cout << "I love You"<<endl;
		}
};	
int main()
{
	harish jp;//jp is an object of the class harish
	jp.hc();
	return 0;
}
