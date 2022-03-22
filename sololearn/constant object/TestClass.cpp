#include "TestClass.h"
#include <iostream>
using namespace std;

TestClass::TestClass()
{
}
void TestClass::TestMethod1()
{
    cout << "hi i am a regular function" << endl;
}

// Constant Function
void TestClass::TestMethod2() const
{ 
    cout << "i am the constant function" << endl;
}
