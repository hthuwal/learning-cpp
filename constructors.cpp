// a constructor is a function that gets called autmatically as soon as the object is created
// they never have a return type
// constructor name is same as the class name
// and is always public
// generally used to give variables of object an intial value

#include <iostream>
#include <string>

using namespace std;

class TestClass
{
public:
	TestClass(string z)
	{
		cout << "a new object is created and is being printed automatically" << endl;
		setName(z);
	}

private: // making variabls public is not good.....makes object vulnerable//so main cant have a direct access to private variable//accessible only to class
	string name;

public: // we can access this function from name as it is public and this function can now change the variabel name which is private so we are accessing it privately
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
	TestClass obj("Test Object 1"); // only decleraing this calls the constructor
	cout << obj.getName() << endl;
	return 0;
}
