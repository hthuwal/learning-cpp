//we hid the private variable along with encapsulation //but provided an alternate indirect way to access it
#include<iostream>
#include<string>

using namespace std;

class Encapsulation
{
	private://making variabls public is not good.....makes object vulnerable//so main cant have a direct access to private variable//accessible only to class
		string name;
	public://we can access this function from main as it is public and this function can now change the variabel name which is private so we are accessing it privately
		void setName(string x)
		{
			name = x;
		}
		string getName()
		{
			return name;
		}
};

int main()
{
	Encapsulation obj;
	//obj.name = "Test";  name is private so cant access it directly
	//cout<<obj.name<<endl;
	obj.setName("Test");
	cout<<obj.getName()<<endl;
	return 0;
}
