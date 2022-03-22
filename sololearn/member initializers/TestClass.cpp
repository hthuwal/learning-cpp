#include "TestClass.h"
#include <iostream>
using namespace std;

// the constructor intializer list is placed between the parantesis and the body !!!
TestClass::TestClass(int a, int b) : regVar(a), constVar(b)
{
}
void TestClass::print()
{
    cout << "regular variable is " << regVar << endl;
    cout << "constant variable is " << constVar << endl;
}
