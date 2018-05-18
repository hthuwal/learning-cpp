// friend of a class is something that is completely seperated from the class but still has access to the stuffs inside the class//
#include <iostream>
using namespace std;

class TestClass
{
public:
    TestClass()
    {
        member = 0;
    }

private:
    int member;
    friend void friend_function(TestClass &test);
};

// even though the friend functionn is outside the class it still has access to the stuffs instide the class even the private variables as it is the friend of the class
void friend_function(TestClass &test)
{
    test.member = 99;
    cout << test.member << endl;
}

int main()
{
    TestClass test_obj;
    friend_function(test_obj);
}
